//
// Created by giova on 4/10/2022.
//

#ifndef INC_22S_FINAL_PROJ_SEARCHENGINE_H
#define INC_22S_FINAL_PROJ_SEARCHENGINE_H

#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>

#include "rapidjson/document.h"
#include "porter2_stemmer.h"

#include "AVLTree.h"
#include "Index.h"

using namespace std;
namespace fs = std::filesystem;

class SearchEngine {
private:

    /// Index variables
    AVLTree<Index> index;
    AVLTree<Index> persons;
    AVLTree<Index> organizations;
    AVLTree<string> stopwords;

    /// Temporary and global variables
    vector<Document> resultsList;
    string path;

    /// Stats variables
    unsigned int docsParsed = 0;
    string longestWord = "";
    double lastParseTime = 0;
    AVLTree<WordFrequency> wordFrequency;

    /// Private functions
    /**
     * Loads the stop words contained in "stopwords.txt" into the
     * stopwords tree.
     * @return Void.
     **/
    void loadStopWords(string);

    /**
     * Processes single or boolean queries. Returns a boolean
     * variable indicating if the search was successful.
     * @return Boolean.
     **/
    bool processQuery(string);

    /**
     * Calculates the intersection of two Document arrays and stores
     * the result into the results tree.
     * @return Void.
     **/
    void calculateIntersection(Document* arr1, Document* arr2, int len1, int len2);
    /**
     * Calculates the union of two Document arrays and stores the
     * result into the results tree.
     * @return Void.
     **/
    void calculateUnion(Document* arr1, Document* arr2, int len1, int len2);
    /**
     * Calculates the difference between an array of Documents and
     * the results tree.
     * @return Void.
     **/
    void calculateDifference(Document* arr, int len);

public:

    /// MENU
    /**
     * Displays and executes the basic menu functions of the search
     * engine. Returns boolean indicating if user exits the program.
     * @return Boolean.
     **/
    bool menu();

    /// ENGINE FEATURES
    /**
     * Parses all the documents in the root path to the index tree.
     * Returns the time taken on the parse in seconds.
     * @return Double.
     **/
    double parse(string);

    /**
     * Processes and displays a query. Returns a boolean
     * variable indicating if the search was successful.
     * @return Boolean.
     **/
    bool search(string);

    /**
     * Writes the index persistence to "index_persistence.dat".
     * @return Void.
     **/
    void writePersistence();

    /**
     * Clears the index persistence.
     * @return Void.
     **/
    void clearPersistence();

    /**
     * Reads the index persistence contained in
     * "index_persistence.dat" and stores it in the index tree.
     * @return Void.
     **/
    void readPersistence();

    /// SETTERS AND GETTERS
    /**
     * Sets root path containing the documents.
     * @return Void.
     **/
    void setPath(string);
};

#endif //INC_22S_FINAL_PROJ_SEARCHENGINE_H
