// upvmarker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CSVRow.h"
#include <iostream>
#include <set>
#include <stdlib.h>

std::string gen_unique_str(std::string _1, std::string _2) {
	return "\""+ _1 + "\", \"" + _2 + "\"";
}

std::set<std::string> visits, captures;


// function origin from https://stackoverflow.com/a/874160
bool hasEnding(std::string const &fullString, std::string const &ending) {
	return fullString.length() >= ending.length() ? (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)) : false;
}


int main()
{
	CSVRow csvrow;
	std::ifstream file("../game_log.tsv");
	if (file.is_open())
		std::cout << "Processing....\n";
	else {
		file.open("game_log.tsv");
		if (!file.is_open()) {
			std::cerr << "File not found or can't open\n";
#ifdef _WIN32
			system("pause");
#endif
			return 1;
		}
		else
			std::cout << "Processing....\n";
	}
	while (file >> csvrow) {
		if (csvrow[3] == "captured portal" && csvrow[4] != "failed")
			captures.insert(gen_unique_str(csvrow[1], csvrow[2]));

		if (csvrow[4] != "failed" && (csvrow[3].find("hacked") == 0 && hasEnding(csvrow[3], "portal")) || hasEnding(csvrow[3], "deployed"))
			visits.insert(gen_unique_str(csvrow[1], csvrow[2]));
	}
	file.close();
	//std::cout << captures.size() << std::endl;
	//std::cout << visits.size();
	std::ofstream fout("../html/data.js");
	if (!fout.is_open()){
		fout.open("html/data.js");
		if (!fout.is_open()) {
			std::cerr << "open output file error\n";
#ifdef _WIN32
			system("pause");
#endif
			return 1;
		}
	}
	fout << "result = {\"visits\": [";
	for (std::string s : visits)
		fout << "[" << s << "], ";
	fout << "], \"captures\": [";
	for (std::string s : captures)
		fout << "[" << s << "], ";
	fout << "]}";
	fout.close();
	std::cout << "Done!\nPlease open \"html / index.html\" to view your upv map\n";
#ifdef _WIN32
	system("pause");
#endif
	return 0;
}