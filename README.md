# Search Engine

## Abstract

Search engine for a large collection of financial news articles. All documents (JSON formatted) are processed by the document parser, which removes all stop words, and steams the words using porter2 stemmer. Then, the query processor parses simple prefix Boolean queries entered by the user in the search engine. A result list is returned, sorted by relevancy (term frequency).

## Examples

* markets
    * This query should return all articles that contain the word markets.
* AND social network
    * This query should return all articles that contain the words “social” and “network” (doesn’t have to be as a 2-word phrase)
* AND social network PERSON cramer
    * This query should return all articles that contain the words social and network and that mention cramer as a person entity.
* AND social network ORG facebook PERSON cramer
    * This query should return all articles that contain the words social and network, that have an entity organization of facebook and that mention cramer as a person entity.
* OR snap facebook
    * This query should return all articles that contain either snap OR facebook
* OR facebook meta NOT profits
    * This query should return all articles that contain facebook or meta but that do not contain the word profits. 
* bankruptcy NOT facebook
    * This query should return all articles that contain bankruptcy, but not facebook.
* OR facebook instagram NOT bankruptcy ORG snap PERSON cramer
    * This query should return any article that contains the word facebook OR instagram but that does NOT contain the word bankruptcy, and the article should have an organization entity with Snap and a person entity of cramer

## Additional Features

An AVL Tree class was implemented from scratch and test cases where created to check the functionality of the class.

[Handout](https://docs.google.com/document/d/1210mEIwg2rVnId4POk5gmaWFZ3mD27dts4Kwh4RTBbA/edit)
