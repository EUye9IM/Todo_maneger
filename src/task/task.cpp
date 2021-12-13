#include "task.h"
#include "b64/b64.h"
#include <fstream>
#include <iostream>
#include <sstream>
int printTask(const Tasks &tasks) {
	std::string fmt;
	char buf[64];
	int namelen = 0;
	for (Task t : tasks) {
		if (namelen < int(t.name.length()))
			namelen = int(t.name.length());
	}
	namelen += 10;
	printf("\n");
	fmt = "%8s%" + std::to_string(namelen) + "s%12s\n";
	printf(fmt.c_str(), "No.", "task name", "deadLine");
	fmt = "%8d%" + std::to_string(namelen) + "s%12s\n";
	printf("\n");
	for (int i = 0; i < int(tasks.size()); i++) {
		tm tt;
		localtime_s(&tt, &tasks[i].ddl);
		strftime(buf, 64, "%m-%d %Y", &tt);
		printf(fmt.c_str(), i, tasks[i].name.c_str(), buf);
	}
	printf("\n");
	return 0;
}
int readTask(const std::string &path, Tasks &tasks) {
	tasks.clear();
	std::ifstream file(path);
	if (!file.is_open())
		return -1;
	while (!file.eof()) {
		std::string line;
		std::getline(file, line);
		if (file.eof()) {
			file.close();
			return 0;
		}

		Task t;
		std::stringstream ss(line);
		unsigned char *s1, *s2;
		ss >> t.name >> t.ddl;
		ss >> t.info;
		s1 = b64_decode(t.name.c_str(), t.name.length());
		s2 = b64_decode(t.info.c_str(), t.info.length());
		t.name = std::string((char *)s1);
		t.info = std::string((char *)s2);
		tasks.push_back(t);
		free(s1);
		free(s2);
	}
	file.close();
	return 0;
}
int saveTask(const std::string &path, const Tasks &tasks) {
	std::ofstream file(path);
	if (!file.is_open())
		return -1;
	for (Task t : tasks) {
		char *s1, *s2;
		s1 = b64_encode((const unsigned char *)t.name.c_str(), t.name.length());
		s2 = b64_encode((const unsigned char *)t.info.c_str(), t.info.length());
		file << s1 << " " << t.ddl << " " << s2 << std::endl;
		free(s1);
		free(s2);
	}
	file.close();

	return 0;
}