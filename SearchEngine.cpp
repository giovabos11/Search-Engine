//
// Created by giova on 4/10/2022.
//

#include "SearchEngine.h"

bool SearchEngine::menu() {
    int choice;
    bool repeat = true;
    string query;
    string inputPath;

    cout << endl;
    cout << " ------------ Search Engine ------------" << endl << endl;
    cout << "1. Enter search query" << endl;
    cout << "2. Generate index" << endl;
    cout << "3. Clear index" << endl;
    cout << "4. Write index to persistence" << endl;
    cout << "5. Clear index persistence" << endl;
    cout << "6. Read index persistence" << endl;
    cout << "7. Statistics" << endl;
    cout << "8. Exit" << endl << endl;
    cout << "Enter your choice: ";

    // Check if input is a number
    if (!(cin >> choice)) {
        cout << endl << "Invalid choice." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return repeat;
    }

    cout << endl;

    switch(choice) {
        // Search query
        case 1: {
            cout << "> ";
            getline(cin, query);
            getline(cin, query);
            cout << endl;
            // If search is successful
            if (search(query)) {
                // Ask the user to open a document
                do {
                    cout << "Choose a document number or enter 0 to exit: ";
                    // If user enters a non-int value
                    if (!(cin >> choice)) {
                        cout << endl << "Invalid choice." << endl << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        choice = -1;
                        continue;
                    }
                    cout << endl;
                    // Handle list of documents
                    if (choice > 0 && choice <= resultsList.size()) {
                        rapidjson::Document doc;
                        ifstream inputFile;
                        string temp = "";
                        string wholeFile = "";

                        inputFile.open(resultsList[choice - 1].docTitle);

                        // Put the whole file's data into wholeFile
                        while(getline(inputFile, temp)) {wholeFile+= temp;}

                        // Close the input file buffer
                        inputFile.close();

                        // Call parse on the string
                        doc.Parse(wholeFile.c_str());

                        // Get news text
                        cout << doc["text"].GetString() << endl << endl;
                    }
                    else if (choice != 0) {
                        cout << "Invalid choice." << endl << endl;
                    }
                } while (choice != 0);
            }
        } break;
        // Generate index
        case 2: {
            cout << "Enter data folder path: ";
            cin >> inputPath;
            cout << endl;
            // Check if path exists
            if (fs::exists(inputPath)) {
                // Set path
                path = inputPath;
                // Clear index
                index.clear();
                // Reset longest word
                longestWord = "";
                // Parse documents in path
                lastParseTime = parse(path);
                cout << "All data successfully parsed! (" << lastParseTime << " seconds)" << endl;
            }
            else {
                cout << "Invalid path." << endl << endl;
            }
        } break;
        // Clear index
        case 3: {
            // Clear index
            index.clear();
            // Reset longest word
            longestWord = "";
            cout << "Index cleared!" << endl;
        } break;
        // Write index to persistence
        case 4: {
            writePersistence();
            cout << "Index wrote to persistence!" << endl;
        } break;
        // Clear index persistence
        case 5: {
            clearPersistence();
            cout << "Index persistence cleared!" << endl;
        } break;
        // Read index persistence
        case 6: {
            readPersistence();
            cout << "Index persistence read!" << endl;
        } break;
        // Statistics
        case 7: {
            cout << "Total documents parsed: " << docsParsed << endl;
            cout << "Total words in index: " << index.getSize() <<  endl;
            cout << "Total unique persons: " << persons.getSize() <<  endl;
            cout << "Total unique organizations: " << organizations.getSize() <<  endl;
            cout << "Last parse time: " << lastParseTime << " seconds" << endl;
            cout << "Longest word in index: " << longestWord << " (" << longestWord.length() << " characters)" <<  endl;
            if (wordFrequency.getSize() > 0) {
                cout << "Top 25 most frequent words: " << endl;
                // Store word frequency in temporary list
                WordFrequency* topWords = wordFrequency.getList();

                // Sort word frequency using bubble sort
                bool done = false;
                for (int i = 0; i < wordFrequency.getSize() - 1 && !done; i++) {
                    done = true;
                    for (int j = 0; j < wordFrequency.getSize() - 1 - i; j++) {
                        if (topWords[j].frequency < topWords[j + 1].frequency) {
                            swap(topWords[j], topWords[j + 1]);
                            done = false;
                        }
                    }
                }

                // Print top 25 words by frequency
                for (int i = 0; i < wordFrequency.getSize() && i < 25; i++) {
                    cout << (i+1) << ". " << topWords[i].word << " (" << topWords[i].frequency << " occurrences)" << endl;
                }

                // Delete temporary top words list
                delete[] topWords;
            }
        } break;
        // Exit
        case 8: {
            repeat = false;
        } break;
        // Choice not found
        default: {
            cout << "Invalid choice." << endl;
        } break;
    }

    return repeat;
}

double SearchEngine::parse(string directory) {
    // Create a start and end time_point object
    chrono::time_point<chrono::high_resolution_clock> chr_start, chr_end;

    // Start time record
    chr_start = chrono::high_resolution_clock::now();

    rapidjson::Document doc;
    string wholeFile = "";
    string temp = "";
    string text = "";
    string docID = "";

    stringstream splitter;
    string token = "";

    Index tempIndex;

    ifstream inputFile;

    // Reset longest word
    longestWord = "";

    // Clear word frequency counter
    wordFrequency.clear();

    // Load stop words
    loadStopWords("/mnt/c/Users/giova/Desktop/data/stopwords.txt");

    // For every file in every folder
    for (const auto & entry : fs::recursive_directory_iterator(directory)){
        if (entry.is_regular_file()) {
            if (entry.path().extension().string() == ".json") {
                // Open current file
                inputFile.open(entry.path().c_str());

                // Put the whole file's data into wholeFile
                while(getline(inputFile, temp)) {wholeFile+= temp;}

                // Close the input file buffer
                inputFile.close();

                // Call parse on the string
                doc.Parse(wholeFile.c_str());

                // Get news text
                text = doc["text"].GetString();

                // Lower case and clean the text
                for (int i = 0; i < text.length(); i++) {
                        text[i] = tolower(text[i]);
                }

                // Get document id
                docID = doc["uuid"].GetString();

                // Store persons
                for (auto &v: doc["entities"]["persons"].GetArray()) {
                    // Split persons text
                    splitter.str(v["name"].GetString());
                    while (splitter >> tempIndex.word) {
                        // Insert the person name into indexes, store document ID and name, and increment frequency
                        persons.insert(tempIndex).docs.insert(Document(docID, entry.path().c_str())).frequency++;
                    }
                }

                // Clear string stream
                splitter.clear();

                // Store organizations
                for (auto &v: doc["entities"]["organizations"].GetArray()) {
                    // Split persons text
                    splitter.str(v["name"].GetString());
                    while (splitter >> tempIndex.word) {
                        // Insert the person name into indexes, store document ID and name, and increment frequency
                        organizations.insert(tempIndex).docs.insert(Document(docID, entry.path().c_str())).frequency++;
                    }
                }

                // Clear string stream
                splitter.clear();

                // Split text
                splitter.str(text);
                while (splitter >> tempIndex.word) {
                    // Search for stop word
                    if (stopwords.contains(tempIndex.word)) { continue; }

                    // Stem word
                    Porter2Stemmer::stem(tempIndex.word);

                    // Insert the index into indexes, store document ID and name, and increment frequency
                    index.insert(tempIndex).docs.insert(Document(docID, entry.path().c_str())).frequency++;

                    // Check for longest word
                    if (tempIndex.word.length() > longestWord.length()) {
                        longestWord = tempIndex.word;
                    }

                    // Store word frequency
                    wordFrequency.insert(WordFrequency(tempIndex.word)).frequency++;
                }
                // Clear string stream
                splitter.clear();

                // Reset temp and whole file variables to store new data
                temp = "";
                wholeFile = "";

                // Increment documents parsed
                docsParsed++;
            }
        }
    }

    // Stop time record and return it
    chr_end = chrono::high_resolution_clock::now();
    chrono::duration<double> time_in_seconds = chr_end - chr_start;
    return time_in_seconds.count();
}

void SearchEngine::loadStopWords(string filename) {
    string stopword;
    ifstream inputFile;

    // Open stopwords file
    inputFile.open(filename);

    // If file exist
    if (inputFile) {
        // Get each stop word
        while (getline(inputFile, stopword)) {
            // Insert stop word
            stopwords.insert(stopword);
        }
        // Close input file
        inputFile.close();
    }
}

bool SearchEngine::search(string query) {
    // Create a start and end time_point object
    chrono::time_point<chrono::high_resolution_clock> chr_start, chr_end;

    // Start time record
    chr_start = chrono::high_resolution_clock::now();

    // If search query is in index
    if (processQuery(query)) {
        // Stop time record
        chr_end = chrono::high_resolution_clock::now();
        chrono::duration<double> time_in_seconds = chr_end - chr_start;

        cout << "Results for \"" << query << "\" (" << time_in_seconds.count() << " seconds): " << endl << endl;

        // Sort documents by frequency using bubble sort
        bool done = false;
        for (int i = 0; i < resultsList.size() - 1 && !done; i++) {
            done = true;
            for (int j = 0; j < resultsList.size() - 1 - i; j++) {
                if (resultsList[j].frequency < resultsList[j + 1].frequency) {
                    swap(resultsList[j], resultsList[j + 1]);
                    done = false;
                }
            }
        }

        // Print results
        for (int i = 0; i < resultsList.size(); i++) {
            cout << (i + 1) << ". " << resultsList[i] << endl;
        }
        cout << endl;

        return true;
    }
    // Otherwise, there are no matches
    else {
        cout << "No results found for \"" << query << "\"." << endl;
        return false;
    }
}

bool SearchEngine::processQuery(string query) {
    stringstream splitter;
    string token;
    string prevBool = "";
    Index tempIndex;

    vector<Index> intersectionList;
    vector<Index> unionList;
    vector<Index> differenceList;

    Document* arr1;
    Document* arr2;

    // Clear results list
    resultsList.clear();

    // Load string stream with query
    splitter.str(query);

    // Split query into tokens
    while (splitter >> token) {
        // Check if token is a boolean expresion
        if (token == "AND" || token == "OR" || token == "NOT" || token == "PERSON" || token == "ORG") {
            prevBool = token;
        }
        // Handle not boolean token
        else {
            // Lower case and clean the text
            for (int i = 0; i < token.length(); i++) {
                token[i] = tolower(token[i]);
            }
            // Stem word
            Porter2Stemmer::stem(query);

            // Store word into temporary index
            tempIndex.word = token;

            // If previous boolean token is AND
            if (prevBool == "AND") {
                // If index tree contains the word
                if (index.contains(tempIndex)) {
                    // Push that index to the intersection list
                    intersectionList.push_back(index.search(tempIndex));
                }
                // Otherwise, the boolean token AND is active and the element was
                // not found in the index, so there are no results automatically
                else {
                    return false;
                }
            }
            // If previous boolean token is OR
            else if (prevBool == "OR") {
                // If index tree contains the word
                if (index.contains(tempIndex)) {
                    // Push that index to the union list
                    unionList.push_back(index.search(tempIndex));
                }
            }
            // If previous boolean token is NOT
            else if (prevBool == "NOT") {
                // If index tree contains the word
                if (index.contains(tempIndex)) {
                    // Push that index to the difference list
                    differenceList.push_back(index.search(tempIndex));
                }
            }
            // If previous boolean token is PERSON
            else if (prevBool == "PERSON") {
                // If persons tree contains the word
                if (persons.contains(tempIndex)) {
                    // Push that index to the intersection list
                    intersectionList.push_back(persons.search(tempIndex));
                }
                // Otherwise, the boolean token PERSON is active and the element was
                // not found in the index, so there are no results automatically
                else {
                    return false;
                }
            }
            else if (prevBool == "ORG") {
                // If organizations tree contains the word
                if (organizations.contains(tempIndex)) {
                    // Push that index to the intersection list
                    intersectionList.push_back(organizations.search(tempIndex));
                }
                // Otherwise, the boolean token ORG is active and the element was
                // not found in the index, so there are no results automatically
                else {
                    return false;
                }
            }
            // Handle search with no boolean expression
            else {
                // If index tree contains the word
                if (index.contains(tempIndex)) {
                    // Push that index to the intersection list
                    intersectionList.push_back(index.search(tempIndex));
                }
                // Otherwise, the single element was not found in the index,
                // so there are no results automatically
                else {
                    return false;
                }
            }
        }
    }

    // Calculate union if list is not empty
    if (!unionList.empty()) {
        // If there are 2 arguments
        if (unionList.size() == 2) {
            // Store document lists in their respective arrays
            arr1 = unionList[0].docs.getList();
            arr2 = unionList[1].docs.getList();
            // Calculate union between the two arrays
            calculateUnion(arr1,
                           arr2,
                           unionList[0].docs.getSize(),
                           unionList[1].docs.getSize());
            // Delete the arrays
            delete[] arr1;
            delete[] arr2;
        }
        // If there is 1 argument (single query)
        else if (unionList.size() == 1) {
            // Store the same document in the two arrays
            arr1 = unionList[0].docs.getList();
            arr2 = unionList[0].docs.getList();
            // Calculate the union between the same array
            calculateUnion(arr1,
                           arr2,
                           unionList[0].docs.getSize(),
                           unionList[0].docs.getSize());
            // Delete the arrays
            delete[] arr1;
            delete[] arr2;
        }
        // Otherwise, arguments count is not valid
        else {
            return false;
        }
    }

    // Calculate intersection if list is not empty
    if (!intersectionList.empty()) {
        // If there are 2 arguments
        if (intersectionList.size() >= 2) {
            // Store document lists in their respective arrays
            arr1 = intersectionList[0].docs.getList();
            arr2 = intersectionList[1].docs.getList();
            // Calculate the intersection between the two arrays
            calculateIntersection(arr1,
                                  arr2,
                                  intersectionList[0].docs.getSize(),
                                  intersectionList[1].docs.getSize());
            // Delete arrays
            delete[] arr1;
            delete[] arr2;
            // If there are more than two intersections
            for (int i = 2; i < intersectionList.size(); i++) {
                // Store a temporary list of results
                Document tempResults[resultsList.size()];
                for (int j = 0; j < resultsList.size(); j++) {
                    tempResults[j] = resultsList[j];
                }
                // Store subsequent document list in an array
                arr2 = intersectionList[i].docs.getList();
                // Calculate the intersection between the results list and
                // the subsequent list of documents
                calculateIntersection(tempResults,
                                      arr2,
                                      resultsList.size(),
                                      intersectionList[i].docs.getSize());
                // Delete the array
                delete[] arr2;
            }
        }
        // If there is 1 argument (single query)
        else if (intersectionList.size() == 1) {
            // If results list is not empty
            if (!resultsList.empty()) {
                // Store a temporary list of results
                Document tempResults[resultsList.size()];
                for (int j = 0; j < resultsList.size(); j++) {
                    tempResults[j] = resultsList[j];
                }
                // Store the document list in an array
                arr2 = intersectionList[0].docs.getList();
                // Calculate the intersection between the results list and
                // the list of documents
                calculateIntersection(tempResults,
                                      arr2,
                                      resultsList.size(),
                                      intersectionList[0].docs.getSize());
                // Delete the array
                delete[] arr2;
            }
            // Otherwise, calculate the intersection for the same word
            else {
                // Store the same document in the two arrays
                arr1 = intersectionList[0].docs.getList();
                arr2 = intersectionList[0].docs.getList();
                // Calculate the union between the same array
                calculateIntersection(arr1,
                                      arr2,
                                      intersectionList[0].docs.getSize(),
                                      intersectionList[0].docs.getSize());
                // Delete the arrays
                delete[] arr1;
                delete[] arr2;
            }
        }
        // Otherwise, arguments count is not valid
        else {
            return false;
        }
    }

    // If differenceList is not empty, calculate difference
    if (!differenceList.empty()) {
        // Store document list in an array
        arr1 = differenceList[0].docs.getList();
        // Calculate difference between the results list and the difference list
        calculateDifference(arr1,
                            differenceList[0].docs.getSize());
        // Delete the array
        delete[] arr1;
    }

    // If the result list is empty
    if (resultsList.empty()) {
        // Return false
        return false;
    }
    // Otherwise, there is at least one result
    else {
        // Return true
        return true;
    }
}

void SearchEngine::calculateIntersection(Document* arr1, Document* arr2, int len1, int len2) {
    int i, j;

    // Clear the results list
    resultsList.clear();

    // Calculate the intersection of the two arrays and store it into the results list
    for (i = 0; i < len1; i++) {
        for (j = 0; j < len2; j++) {
            if (arr1[i] == arr2[j]) {
                resultsList.push_back(arr1[i]);
            }
        }
    }
}

void SearchEngine::calculateUnion(Document* arr1, Document* arr2, int len1, int len2) {
    bool flag;
    int i, j;

    // Push the first array to the results list
    for (i = 0; i < len1; i++) {
        resultsList.push_back(arr1[i]);
    }

    // Calculate union between the results list and the second array
    for (i = 0; i < len2; i++) {
        flag = false;
        for (j = 0; j < len1; j++) {
            if (arr2[i] == arr1[j]) {
                flag = true;
            }
        }
        if (!flag) {
            resultsList.push_back(arr2[i]);
        }
    }
}

void SearchEngine::calculateDifference(Document* arr, int len) {
    int i, j;

    // Calculate difference between the results list and the array provided
    for (i = 0; i < len; i++) {
        for (j = 0; j < resultsList.size(); j++) {
            if (arr[i] == resultsList[j]) {
                resultsList.erase(resultsList.begin() + j);
            }
        }
    }
}

void SearchEngine::writePersistence() {
    // Create a new output buffer
    ofstream outputFile;

    // Create a new persistence file
    outputFile.open("index_persistence.dat");

    // If file is successfully created
    if (outputFile) {
        // Temporarily store index into a list
        Index* indexList = index.getList();

        // Generate data file
        for (int i = 0; i < index.getSize(); i++) {
            outputFile << indexList[i].word;
            Document* documentList = indexList[i].docs.getList();
            for (int j = 0; j < indexList[i].docs.getSize(); j++) {
                outputFile << "|" << documentList[j].docID << "|" << documentList[j].docTitle << "|" << documentList[j].frequency;
            }
            delete[] documentList;
            outputFile << endl;
        }

        // Delete temporary index list
        delete[] indexList;
    }

    // Close output file buffer
    outputFile.close();
}

void SearchEngine::clearPersistence() {
    // Create a new output buffer
    ofstream outputFile;

    // Create a new persistence file
    outputFile.open("index_persistence.dat");

    // If file is successfully created
    if (outputFile) {
        // Overwrite with a new line
        outputFile << endl;
    }

    // Close output file buffer
    outputFile.close();
}

void SearchEngine::readPersistence() {

}

void SearchEngine::setPath(string arg) {
    // Set path to argument
    path = arg;
}
