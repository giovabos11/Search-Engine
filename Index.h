//
// Created by giova on 4/12/2022.
//

#ifndef INC_22S_FINAL_PROJ_INDEX_H
#define INC_22S_FINAL_PROJ_INDEX_H

#include <string>

using namespace std;

/**
 * Stores the document ID, document title (path), and frequency for a
 * word in an specific document.
 **/
struct Document {
    string docID;
    string docTitle;
    int frequency = 0;

    Document() {}

    Document(string docID, string docTitle) { //, int frequency = 0) {
        this->docID = docID;
        this->docTitle = docTitle;
        this->frequency = 0;
    }

    Document& operator=(const Document& d) {
        this->docID = d.docID;
        this->docTitle = d.docTitle;
        this->frequency = d.frequency;
        return *this;
    }

    bool operator==(const Document& d) const {
        return docID == d.docID;
    }

    bool operator<(const Document& d) const {
        return docID < d.docID;
    }

    bool operator>(const Document& d) const {
        return docID > d.docID;
    }

    friend ostream& operator<<(ostream& out, const Document& d) {
        out << "Document ID: " << d.docID << " | Document title: " << d.docTitle << " | Frequency: " << d.frequency;
        return out;
    }
};

/**
 * Stores the word and a list of documents with its respective
 * frequency where the word appears.
 **/
struct Index {
    string word;
    AVLTree<Document> docs;

    Index& operator=(const Index& i) {
        this->word = i.word;
        this->docs = i.docs;
        return *this;
    }

    bool operator==(const Index& i) const {
        return word == i.word;
    }

    bool operator<(const Index& i) const {
        return word < i.word;
    }

    bool operator>(const Index& i) const {
        return word > i.word;
    }
};

/**
 * Stores the global (entire data set) frequency of each word.
 **/
struct WordFrequency {
    string word;
    int frequency;

    WordFrequency() {
        frequency = 0;
    }

    WordFrequency(string word) {
        this->word = word;
        frequency = 0;
    }

    WordFrequency& operator=(const WordFrequency& w) {
        this->word = w.word;
        this->frequency = w.frequency;
        return *this;
    }

    bool operator==(const WordFrequency& w) const {
        return word == w.word;
    }

    bool operator<(const WordFrequency& w) const {
        return word < w.word;
    }

    bool operator>(const WordFrequency& w) const {
        return word > w.word;
    }
};

#endif //INC_22S_FINAL_PROJ_INDEX_H
