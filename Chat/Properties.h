/*
 * Properties.h
 *
 *  Created on: 2011-12-26
 *      Author: Dimitri
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <map>

class Properties {
private:
	char* filename;
	std::map<char*, char*> properties;
public:
	virtual ~Properties();
	Properties();
	Properties(char* filename);
	void setFileName(char* filename);
	char* getFileName();
	virtual void loadProperties();
	virtual void loadProperties(char* filename);
	virtual void saveProperties(char* filename);
	char* getProperty(char* key);
	void addProperty(char* key, char* value);
	void removeProperty(char * key);
	bool doesPropertyExist(char * key);
	std::map<char*, char*> getProperties();
	void list();
	void clear();

	template<class Key, class Value, class Comparator, class Alloc>
	void listProperies(const std::map<Key, Value, Comparator, Alloc> my_map);

	template<class Key, class Value, class Comparator, class Alloc>
	bool getValue(const std::map<Key, Value, Comparator, Alloc>& my_map,Key key
			,Value& value);
};

#endif /* PROPERTIES_H_ */
