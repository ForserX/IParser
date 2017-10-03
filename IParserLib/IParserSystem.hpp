#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <list>

class config
{
	using string = std::string;
	using string_view = std::string_view;
public:
	struct section
	{
		std::string name;
		std::string parent;
		std::unordered_map<std::string, std::string> keyvalues;
	};

public:
	config() { }
	config(const string_view filename);

	void WriteSect(const string_view filename, const string_view sectionname, const string_view keyname, const string_view parent = nullptr);
	section* get_section(const string& sectionname);
	inline std::list<section>& get_sections() { return sections; };

	bool	get_logic(const string& sectionname, const string& keyname);
	string	get_value(const string& sectionname, const string& keyname);

private:
	void parse(const std::string& filename);
private:
	std::list<section> sections;
	section currentsection;
};