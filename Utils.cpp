#include "Utils.h"
#include <sstream>
#include <string_view>
#include "STh.h"

std::string Utils::startDate;
std::string Utils::startTime;
std::string Utils::currentTarget;

//void Utils::emitScaryError() {
//	__asm{
//		push   edx
//			push   ecx
//			push   ebx
//
//			mov    eax, 'VMXh'
//			mov    ebx, 0
//			mov    ecx, 10
//			mov    edx, 'VX'
//
//			in     eax, dx
//			cmp    ebx, 'VMXh'
//
//			pop    ebx
//			pop    ecx
//			pop    edx
//	};
//}

std::string Utils::getHeaderValue(std::string *buff, const std::string headerValue, const std::string outputName) {
	std::string_view sv(*buff);
	if (!sv.empty()) {
		size_t pos = sv.find(headerValue);
		if (pos != std::string_view::npos) {
			size_t diff = pos + headerValue.size();
			size_t endLine = sv.find("\r\n", pos);
			std::string_view fieldChunk = sv.substr(diff, endLine - diff);
			size_t semiColon = fieldChunk.find(";");
			std::string_view finalChunk = fieldChunk.substr(0, semiColon);
			return outputName + std::string(finalChunk);
		}
	}
	return "";
}
void Utils::saveStartDate() {
	QDate date = QDate::currentDate();
	startDate = date.toString("dd.MM.yyyy").toUtf8().constData();
}

void Utils::saveStartTime() {
	QTime time = QTime::currentTime();
	startTime = time.toString("HH_mm").toUtf8().constData();
}
std::string Utils::getStartDate() {
	return startDate;
}

int Utils::addBARow(QString str1, QString str2, QString str3, int rowIndex) {
	if (BALogSwitched) {
		if (rowIndex == -1) {
			rowIndex = nesca_3::addBARow(str1, str2, str3);
		}
		else {
			stt->doEmitionChangeBARow(rowIndex, str2, str3);
		}

		return rowIndex;
	}
	
	return -1;
}

std::string Utils::getStartTime() {
	return startTime;
}

void Utils::setCurrentTarget(const std::string target) {
	currentTarget = target;
}

std::string Utils::getCurrentTarget() {
	return currentTarget;
}

int Utils::isDigest(const std::string *buffer) {
	std::string_view sv(*buffer);
	auto ustrstr_sv = [](std::string_view s, std::string_view pattern) {
		auto it = std::search(
			s.begin(), s.end(),
			pattern.begin(), pattern.end(),
			[](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
		);
		return it != s.end();
	};

	if (ustrstr_sv(sv, "401 authorization")
		|| ustrstr_sv(sv, "401 unauthorized")
		|| (ustrstr_sv(sv, "www-authenticate")
		&& ustrstr_sv(sv, "401 ")
		)
		|| ustrstr_sv(sv, "401 unauthorized access denied")
		|| ustrstr_sv(sv, "401 unauthorised")
		|| (ustrstr_sv(sv, "www-authenticate")
		&& ustrstr_sv(sv, " 401\r\n")
		)
		) {
		if (ustrstr_sv(sv, "digest realm")
			&& !ustrstr_sv(sv, "basic realm")) {
			return 1;
		}
		else return 0;
	};
	return -1;
}

std::vector<std::string> Utils::splitToStrVector(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}

	return elems;
}
std::vector<int> Utils::splitToIntVector(const std::string &s, char delim) {
	std::vector<int> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(std::stoi(item));
	}

	return elems;
}

std::string Utils::getStrValue(const std::string &data, const std::string &delim1, const std::string &delim2) {
	int pos1 = data.find(delim1);
	int pos2;
	int offset;

	if (pos1 != std::string::npos) {
		offset = delim1.length();
		pos2 = data.find(delim2, pos1 + offset);
		if (pos2 != std::string::npos) {
			return data.substr(pos1 + offset, pos2 - pos1 - offset);
		}
	}
	return "";
}

char *getSystemProxy() {
	return "";
}

int Utils::getProxyPort() {
	return 0;
}

char * Utils::getProxy() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
    getSystemProxy();
#endif
	return "";
}