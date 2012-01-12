/*
 * Properties.cpp
 *
 *  Created on: 2011-12-26
 *      Author: Dimitri
 */

#include "Properties.h"
#include <stdio.h>
#include <xml/tinyxml.h>
#include <map>

Properties::~Properties() {

}

Properties::Properties() {

}

Properties::Properties(char* filename) {
	this->filename = filename;
}

void Properties::loadProperties() {
	TiXmlDocument * doc = new TiXmlDocument();
	if (doc->LoadFile(filename, TIXML_ENCODING_UTF8)) {

		char *host = const_cast<char*>(doc->FirstChildElement()->GetText());char *port = const_cast<char*>(doc->FirstChildElement()->NextSiblingElement()->GetText());

		//get host pair
		this->properties.insert(std::pair<char*, char*>("host", host));

		//get port pair
		this->properties.insert(std::pair<char*, char*>("port", port));
	}
	}

void Properties::loadProperties(char* filename) {
	TiXmlDocument * doc = new TiXmlDocument();
	if (doc->LoadFile(filename, TIXML_ENCODING_UTF8)) {

		char *host = const_cast<char*>(doc->FirstChildElement()->GetText());
		char *port = const_cast<char*>(doc->FirstChildElement()->NextSiblingElement()->GetText());

		//get host pair
		this->properties.insert(std::pair<char*, char*>("host", host));

		//get port pair
		this->properties.insert(std::pair<char*, char*>("port", port));
	}
}

void Properties::saveProperties(char * filename) {
	TiXmlDocument * doc = new TiXmlDocument(filename);
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);
	std::map<char*, char*>::iterator it = this->properties.begin();
	for (; it != this->properties.end(); ++it) {
		TiXmlElement * element = new TiXmlElement(it->first);
		TiXmlText * element_text = new TiXmlText(it->second);
		element->LinkEndChild(element_text);
		doc->LinkEndChild(element);
	}
	doc->SaveFile();
}

template<class Key, class Value, class Comparator, class Alloc>
void Properties::listProperies(
		const std::map<Key, Value, Comparator, Alloc> my_map) {

	typename std::map<Key, Value, Comparator, Alloc>::iterator it =
			my_map.begin();

	for (; it != my_map.end(); ++it) {
		printf("%s => %s\n", it->first, it->second);
	}
}

void Properties::list() {

	std::map<char*, char*>::iterator it = this->properties.begin();

	for (; it != this->properties.end(); ++it) {
		printf("%s => %s\n", it->first, it->second);
	}
}

void Properties::clear() {
	this->properties.clear();
}

std::map<char*, char*> Properties::getProperties() {
	return this->properties;
}

void Properties::setFileName(char* filename) {
	this->filename = filename;
}

char* Properties::getFileName() {
	return this->filename;
}

void Properties::addProperty(char* key, char *value) {
	this->properties.insert(std::pair<char*, char*>(key, value));
}

void Properties::removeProperty(char* key) {
	this->properties.erase(key);
}

bool Properties::doesPropertyExist(char* key) {
	std::map<char*, char*>::iterator it = this->properties.find(key);
	return it != this->properties.end();
}

template<class Key, class Value, class Comparator, class Alloc>
bool Properties::getValue(const std::map<Key, Value, Comparator, Alloc>& my_map
		,Key key ,Value& value) {

	typename std::map<Key, Value, Comparator, Alloc>::const_iterator it =
			my_map.find(key);

	if (it != my_map.end()) {
		value = it->second;
		return true;
	}
	return false;
}

char * Properties::getProperty(char * key) {
	std::map<char*, char*>::const_iterator it = this->properties.find(key);
	if (it != this->properties.end()) {
		return it->second;
	}
	return NULL;
}

