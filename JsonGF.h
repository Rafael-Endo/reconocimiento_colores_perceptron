#pragma once
#ifndef JSONGF_H
#define JSONGF_H

#include <vector>
#include <fstream>
#include <random>
#include <limits>
#include <iostream>
#include <string>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

struct JsonGF {

	JsonGF(
		const string &path,
		const vector<vector<double>> &c1WT,
		const vector<vector<double>> &c2WT,
		const vector<vector<double>> &c3WT,
		const vector<double> &c1ET,
		const vector<double>& c2ET,
		const vector<double>& c3ET
		) {
		ifstream input_file(path);
		json GF;
		input_file >> GF;
		GF["clase1WT"] = c1WT; GF["clase2WT"] = c2WT; GF["clase3WT"] = c3WT;
		GF["clase1ET"] = c1ET; GF["clase2ET"] = c2ET; GF["clase3ET"] = c3ET;
		ofstream out_file(path, ios::trunc);
		out_file << GF.dump(4);
	}
};







#endif