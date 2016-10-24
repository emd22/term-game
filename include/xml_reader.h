#ifndef XML_READER_HPP
#define XML_READER_HPP

#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;

#include <string>
using std::string;
#include <cstring>
using std::strcmp;

#include <vector>
using std::vector;

#include <map>
using std::map;

typedef map<string, string> Attributes;

class SaxHandler
{
public:
	SaxHandler() {}

	virtual void beginElement(string name, Attributes attributes) = 0;
	virtual void endElement(string name) = 0;
	virtual void characters(string value) = 0;
	virtual void comment(string comment) = 0;
};

class SaxParser
{
private:
	ifstream file;
	SaxHandler *handler;
public:
	SaxParser(SaxHandler *handler)
	{
		this->handler = handler;
	}

	void parse(const char *filepath)
	{
		file.open(filepath);
		if (!file.is_open())
		{
			cout << "Error opening file stream!";
			return;
		}
		char lastValue;

		bool isReading = false,
			isOpening = false,
			isClosing = false,
			inElement = false,
			inComment = false,
			inCharacters = false,
			inHeader = false,
			inAttributes = false,
			inAttributeValue = false,
			inAttributeName = false;

		string elementStr = "", commentStr = "", valueStr = "";
		vector<std::pair<string, string>> attribs;

		char ch;
		while (file.get(ch))
		{
			if (ch != '\t' && ch != '\n') // dont count white space. counting spaces requires determining if they're necessary.
			{							  // see the bottom [ if (inCharacters...) ].
				if (ch == '<')
				{
					elementStr = "";
					inCharacters = false;

					if (strcmp(valueStr.c_str(), ""))
						handler->characters(valueStr);

					isOpening = true;
					isReading = true;
					inElement = true;
					inAttributes = false;
					isClosing = false;
					valueStr = "";
					attribs.clear();
				}
				else if (ch == '!' && inElement)
				{
					inComment = true;
					commentStr = "";
				}
				else if (ch == '?' && inElement)
				{
					inHeader = true;
				}
				else if (ch == '/' && (inElement || (inAttributes && !inAttributeValue))) // if there is a /, we dont count it was a closing, if in an attribute value.
				{
					isOpening = false;
					isClosing = true;
				}
				else if (ch == '>')
				{
					inCharacters = true;

					if (inComment)
					{
						inComment = false;
						handler->comment(commentStr);
					}
					else if (inHeader)
						inHeader = false;
					else
					{
						if (isOpening || lastValue == '/') // just in case of <thing/>, so we call the opening event too
						{
							Attributes localAttribs;

							if (attribs.size() >= 0)
							{
								for (int i = 0; i < attribs.size(); i++)
								{
									if (strcmp(attribs[i].first.c_str(), "")) // it cant be empty
									{
										localAttribs[attribs[i].first] = attribs[i].second;
									}
								}
							}

							handler->beginElement(elementStr, localAttribs);

							isOpening = false;
						}
						if (isClosing)
							handler->endElement(elementStr);

						inAttributes = false;
						inElement = false;
						isClosing = false;
						isReading = false;

						attribs.clear();
					}
				}

				else
				{
					if (!inComment && !inHeader)
					{
						if (isReading)
						{
							if (inElement)
							{
								if (ch == ' ')
								{
									inElement = false;
									inAttributes = true;
									attribs.push_back(std::make_pair(string(""), string("")));
								}
								else
									elementStr += ch;
							}
							else if (inAttributes && isOpening)
							{
								if (!inAttributeValue && ch == ' ')
									attribs.push_back(std::make_pair(string(""), string("")));
								else if (ch == '\"')
									inAttributeValue = !inAttributeValue;
								else
								{
									std::pair<string, string> &attribStr = attribs[attribs.size() - 1];
									if (!inAttributeValue && ch != '=') // must be in attribute name! dont take in the = sign.
										attribStr.first += ch;
									else if (inAttributeValue)
										attribStr.second += ch;
								}
							}
						}
						else
						{
							if (inCharacters)
							{
								if ((ch != ' ' || (lastValue != ' ' && lastValue != '\n' &&lastValue != '<'))) // probably using spaces as indentation
									valueStr += ch;
							}
						}
					}
					else if (inComment && ch != '-')
						commentStr += ch;
				}
			}
			lastValue = ch;
		}
	}
};
#endif
