#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <unordered_map>
#include <list>

namespace IFS
{

	// trim leading white-spaces
	static std::string_view ltrim(std::string_view s) {
		size_t startpos = s.find_first_not_of(" \t\r\n\v\f");
		if (std::string::npos != startpos)
			s = s.substr(startpos);
		return s;
	}

	// trim trailing white-spaces
	static std::string_view rtrim(std::string_view s) {
		size_t endpos = s.find_last_not_of(" \t\r\n\v\f");
		if (std::string::npos != endpos)
			s = s.substr(0, endpos + 1);
		return s;
	}
	struct section
	{
		std::string name;
		std::unordered_map<std::string, std::string> keyvalues;
	};

	class config
	{
	public:
		config()
		{
		}
		config(const std::string_view filename)
		{
			parse(filename);
		}
		void WriteSect(const std::string_view filename, const std::string_view sectionname, const std::string_view keyname)
		{
			std::ofstream inp(filename.data(), std::ios::in);
			inp << "[" << sectionname << "]" << std::endl << sectionname << " = " << keyname;
			inp.close();
		};
		section* get_section(const std::string_view sectionname)
		{
			std::list<section>::iterator found = std::find_if(sections.begin(), sections.end(), [sectionname](const section& sect) {
				return sect.name.compare(sectionname) == 0; });

			return found != sections.end() ? &*found : 0;
		}

		inline std::list<section>& get_sections(){	return sections; };

		std::string get_value(const std::string_view sectionname, const std::string_view keyname)
		{
			const section* sect = get_section(sectionname);
			if (sect) 
			{
				auto it = sect->keyvalues.find(keyname.data());
				if (it != sect->keyvalues.end())
					return it->second;
			}
			return "Error reading!";
		}

	private:
		void parse(const std::string& filename) 
		{
			section currentsection;
			std::ifstream fstrm;
			fstrm.open(filename.data());

			if (!fstrm)
				throw std::invalid_argument(filename + " could not be opened");

			for (std::string line; std::getline(fstrm, line);)
			{
				// if a comment
				if (!line.empty() && (line[0] == ';' || line[0] == '#')) {
					// allow both ; and # comments at the start of a line

				}
				else if (line[0] == '[') {
					/* A "[section]" line */
					size_t end = line.find_first_of(']');
					if (end != std::string::npos) {

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
				else if (!line.empty()) {
					/* Not a comment, must be a name[=:]value pair */
					size_t end = line.find_first_of("=");
					if (end != std::string::npos) {
						std::string name = line.substr(0, end);
						std::string value = line.substr(end + 1);
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
			if (!currentsection.name.empty()) {
				sections.push_back(currentsection);  // copy
				currentsection.name = "";
				currentsection.keyvalues.clear();
			}
		};

		std::list<section> sections;
	};
}