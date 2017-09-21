#include "IParserSystem.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

// trim leading white-spaces
inline std::string_view ltrim(std::string_view s)
{
	size_t startpos = s.find_first_not_of(" \t\r\n\v\f");
	if (std::string::npos != startpos)
		s = s.substr(startpos);
	return s;
}

// trim trailing white-spaces
inline std::string_view rtrim(std::string_view s) 
{
	size_t endpos = s.find_last_not_of(" \t\r\n\v\f");
	if (std::string::npos != endpos)
		s = s.substr(0, endpos + 1);
	return s;
}

config::config(const string_view filename)
{
	string new_file = "";
	bool isPath = false;
	for (unsigned it = 0; it < filename.length(); it++)
	{
		if (filename[it] == '/' && filename[it + 1] != '/' && !isPath)
		{
			isPath = true;
			new_file += '/';
			new_file += '/';
		}
		else
		{
			isPath = false;
			new_file += filename[it];
		}
	}
	parse(new_file);
}

void config::WriteSect(const string_view filename, const string_view sectionname, const string_view keyname)
{
	std::ofstream inp(filename.data(), std::ios::in);
	inp << "[" << sectionname << "]" << std::endl << sectionname << " = " << keyname;
	inp.close();
};

config::section* config::get_section(const string& sectionname)
{
	auto found = std::find_if(sections.begin(), sections.end(), [sectionname](const section& sect) { return sect.name.compare(sectionname) == 0; });
	if (found != sections.end())
	{
		return &*found;
	}
	else if(currentsection.parent[0] != '#')
	{
		string par = currentsection.parent;
		found = std::find_if(sections.begin(), sections.end(), [par](const section& sect) { return sect.name.compare(par) == 0; });
		if (found != sections.end())
		{
			return &*found;
		}
	}
	return 0;
}

std::string config::get_value(const string& sectionname, const string& keyname)
{
	const section* sect = get_section(sectionname);
	if (sect)
	{
		auto it = sect->keyvalues.find(keyname);
		auto newsect = [] (std::string sect) {if (sect[0] == ' ') sect.erase(0, 1); return sect; };
		if (it != sect->keyvalues.end())
		{
			return newsect(it->second);
		}
		else if ((it = sect->keyvalues.find(keyname + ' ')) != sect->keyvalues.end())
		{
			return newsect(it->second);
		}
	}
	return "Error reading! Section: " + sectionname + " Key: " + keyname;
}

void config::parse(const string& filename)
{
	
	std::ifstream fstrm;
	fstrm.open(filename.data());

	if (!fstrm)
		throw std::invalid_argument(filename + " could not be opened");

	for (string line; std::getline(fstrm, line);)
	{
		// if a comment
		if (!line.empty() && (line[0] == ';' || line[0] == '#')) {
			// allow both ; and # comments at the start of a line

		}
		else if (line[0] == '[') {
			/* A "[section]" line */
			size_t end = line.find_first_of(']');

			if (line.find_first_of(':') != string::npos)
			{
				currentsection.parent = line.erase(0, end + 2);
				line = line.erase(end + 1, line.length() - 1);
			}
			else
			{
				currentsection.parent = "#";
			}
			if (end != string::npos) {

				// this is a new section so if we have a current section populated, add it to list
				if (!currentsection.name.empty()) {
					sections.push_back(currentsection);  // copy
					currentsection.name.clear();  // clear section for re-use
					currentsection.keyvalues.clear();
				}
				currentsection.name = line.substr(1, end - 1);
			}
			else {
				// section has no closing ] char
			}
		}
		else if (!line.empty())
		{
			/* Not a comment, must be a name[=:]value pair */
			size_t end = line.find_first_of("=");
			if (end != string::npos)
			{
				string name = line.substr(0, end);
				string value = line.substr(end + 1);
				ltrim(rtrim(name));
				ltrim(rtrim(value));

				currentsection.keyvalues[name] = value;

			}
			else {
				// no key value delimitter
			}
		}
	} // for


	  // if we are out of loop we add last section
	  // this is a new section so if we have a current section populated, add it to list
	if (!currentsection.name.empty())
	{
		sections.push_back(currentsection);  // copy
		currentsection.name = "";
		currentsection.keyvalues.clear();
	}
};