#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <list>

class config
{
	using string = std::string;
	using string_view = std::string_view;

	struct section
	{
		std::string name;
		std::unordered_map<std::string, std::string> keyvalues;
	};

public:
	config() { }
	config(const string_view filename);

	void WriteSect(const string_view filename, const string_view sectionname, const string_view keyname);
	section* get_section(const string_view sectionname);

	inline std::list<section>& get_sections() { return sections; };

	string get_value(const string_view sectionname, const string& keyname);

private:
	void parse(const std::string& filename);
private:
	std::list<section> sections;
};