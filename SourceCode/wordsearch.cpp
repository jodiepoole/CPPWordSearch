#include "pch.h"
#include "wordsearch.h"

	TextEntry::TextEntry(std::string entry)
	{
		text = entry;
		amount = 1;
		repeat = false;
	}
	//Method increases amount value by 1
	void TextEntry::increaseAmount(void)
	{
		amount++;
	}
	//Sets the entry to repeat
	void TextEntry::isRepeat(void)
	{
		repeat = true;
	}
	//Returns text value
	std::string TextEntry::getText(void)
	{
		return text;
	}
	//returns amount value
	int TextEntry:: getAmount(void)
	{
		return amount;
	}
	//if the entry is not a repeat, it will return the entry and the amount of times it occurs
	void TextEntry::getEntry(void)
	{
		if (repeat == false) {
			std::cout << "\"" << getText() << "\"" << " occurs " << getAmount() << " time(s)." << std::endl;
		}
	}

	//This method checks a position for character breaks and returns a bool based off that
	bool File::hasCharacterBreak(size_t position)
	{
		//loop through all character breaks
		for (int loop = 0; loop != characterBreak.size(); loop++)
			//if fileContent at postion is a character break, return true
			if (characterBreak[loop] == fileContent.at(position))
				return true;
		//if fileContent at position doesn't equal any character breaks, return false
		return false;
	}
	size_t File::searchStringFromPosition(std::string search, size_t position)
	{
		bool wordValid = true;
		//find the next part of the user input from position
		size_t newPosition = fileContent.find(search, position);
		//if there isn't an occurance of the string in the text, return -1 (invalid word)
		if (newPosition > fileContent.length())
			return -1;
		//loops from starting position until the newPosition found
		for (int loop = position; loop != newPosition; loop++)
			//if there is a character break(if position and newposition are not part of the same word), return -1 (invalid word)
			if (hasCharacterBreak(loop))
				return -1;
		//otherwise, return newposition if the word is valid
		return newPosition;
	}
	//This method finds words and returns them as a string
	std::string File::returnWord(size_t position)
	{
		std::string output = "";
		//While loop finds beginning of word based on character breaks (or the start of the text file)
		while (!hasCharacterBreak(position) || position == 1)
		{
			//position is reduced by 1 until the first position -1 is found
			position--;
		}
		//the position is increamented by 1 to get the start of the word
		position++;
		//then loop until the end of the word is found based on character breaks
		do
		{
			//the character at the current position is added to the output string and position is increased
			output += fileContent.at(position);
			position++;
			//if the end of the file is reached, break from the loop
			if (position == fileContent.length()) { break; }
		} while (!hasCharacterBreak(position));
		//return the word found
		return output;
	}
	//This methods returns a bool depending the ending of a string matches an ending string 
	bool File::wordHasEnding(std::string word, std::string ending) {
		//if the ending length is less than or equal to the word length
		if (word.length() >= ending.length())
			//return: compare the word to the ending. This will return true or false depending if the endings are the same
			return (0 == word.compare(word.length() - ending.length(), ending.length(), ending));
		//if the ending is more than the length of the word, return false;
		return false;
	}
	//finds word based on user's input and validates it
	std::string File::getSearchedWord(size_t position, std::vector<std::string> searchTerm, bool startsWithWildcard, bool endsWithWildcard, int loopStart)
	{
		bool valid = true;
		size_t newPosition = position;
		//loops through searchTerm (which contains all strings to be searched for and the wildcards)
		for (int loop = loopStart; loop != searchTerm.size(); loop++)
		{
			//if the entry is not a wildcard the set newPosition to the position of the next string searched for
			if (searchTerm[loop] != "*")
			{
				newPosition = searchStringFromPosition(searchTerm[loop], newPosition);
				//if newPosition = -1 (invalid word), then return ""
				if (newPosition == -1)
					return "";
			}
		}
		//output is set to the word starting at position
		std::string output = returnWord(position);
		//if the searchTerm doesn't start with a wildcard and there isn't a character break before the start of the word, set valid to false
		if (startsWithWildcard == false && !hasCharacterBreak(position - 1))
			valid = false;
		//if the searchTerm doesn't end with a wildcard and the word doesn't end with the last term in searchTerm, set valid to false
		if (endsWithWildcard == false && !(wordHasEnding(output, searchTerm[searchTerm.size() - 1])))
			valid = false;
		//if there is only one entry in searchTerm (no wildcards at all) and output doesn't equal the length of the searchTerm, set valid to false
		if (output.size() != searchTerm[0].size() && searchTerm.size() == 1)
			valid = false;
		//if valid is true, return the word
		if (valid == true)
			return output;
		//otherwise, return ""
		return "";
	}

	//Constructor for File class
	File::File(std::string address)
	{
		//Use enters the name of the text file they want to open
		std::string userInput;
		std::cout << "Make sure the .txt file you wish to access is in the same directory as the .exe of this program" << std::endl;
		std::cout << "Please enter the name of the text file (Do not include .txt): ";
		std::cin >> userInput;
		//the .exe is erased from the address path
		address.erase(address.find_last_of("\\"), address.length() - address.find_last_of("\\"));
		//the user's text file and ".txt" is appended to the end of the path
		address += "\\" + userInput + ".txt";
		//this is set as the file's path
		path = address;
	}
	//This method searchs for every instance of the user input, it will return positions of each occurance to searchPositions
	void File::searchStringForAllPositions(std::string search, std::vector<int>& searchPositions)
	{
		//position is set to the first position
		size_t position = fileContent.find(search);
		//loops until it reaches the end of the string
		while (position != std::string::npos)
		{
			//the position is pushed into searchPositions
			searchPositions.push_back(position);
			//the next position is found
			position = fileContent.find(search, position + search.size());
		}
	}
	//This method takes the fileContent and appends a string to the end of it
	void File::appendFileContent(std::string content)
	{
		fileContent = fileContent + " " + content;
	}
	//This method returns fileContent
	std::string File::getFileContent()
	{
		return fileContent;
	}
	//This method return the path
	std::string File::getPath()
	{
		return path;
	}
	//This method returns a bool based on if a file can be found or not
	bool File::openFile()
	{
		//file is opened at path
		file.open(path, std::ios::in);
		//if the file is opened, inform the user and return true
		if (file.is_open())
		{
			std::cout << "FILE FOUND AND OPENED" << std::endl;
			return true;
		}
		//if the file can't be opened, inform the user and return false
		else
		{
			std::cout << "UNABLE TO FIND OR OPEN FILE" << std::endl;
			return false;
		}
	}
	//This method closes the file
	void File::closeFile()
	{
		file.close();
		std::cout << "FILE CLOSED" << std::endl;
	}
	//This method finds words the user search for and pushes them into an entry (unless the word is pruned through validation)
	void File::createEntry(std::vector<TextEntry>& entriesFound, size_t pos, std::vector<std::string> searchTerm, bool startsWithWildcard, bool endsWithWildcard, std::vector<int> searchPositions, int loopStart)
	{
		//get a occurance of a word searched for
		std::string output = getSearchedWord(pos, searchTerm, startsWithWildcard, endsWithWildcard, loopStart);
		//if the output isn't "" ("" output means invalid) then continue with rest of function
		if (output != "")
		{
			//push the string into entriesFound
			entriesFound.push_back(output);
			//Loop through all of the other entries
			for (size_t loop = 0; loop != entriesFound.size() - 1; ++loop)
			{
				//if there is another occurance of this word in entries, then set this entry to repeat and increase the amount attribute of the orginal entry
				if (entriesFound[loop].getText() == entriesFound.back().getText())
				{
					entriesFound[loop].increaseAmount();
					entriesFound.back().isRepeat();
				}
			}
		}
	}


	//This method searchs the userinput word for characters inbetween wildcards
	std::string SearchWord::searchSection(size_t position)
	{
		std::string output = "";
		//while loop reduceds position by one until the first wildcard or beginning of word is found
		while (word.at(position) != wildcard && position != 0)
		{
			position--;
		}
		//if the current position is a wildcard, increase position by one
		if (word.at(position) == wildcard)
			position++;
		//loops until position reaches a wildcard or the end of the word
		do
		{
			//current character is added to output and position is incremented
			output += word.at(position);
			position++;
			//if the end of the word is reached, break from loop
			if (position == word.length())
				break;
		} while (word.at(position) != wildcard && position != word.length());
		//return the string found
		return output;
	}

	//Constructor for searchword
	SearchWord::SearchWord(std::string userInput)
	{
		std::string stringWildcard = "";
		stringWildcard += wildcard;
		startsWithWildcard = true;
		endsWithWildcard = true;
		word = userInput;
		//Finds the first wildcard in the userinput
		size_t position = userInput.find(wildcard);
		//if the userinput doesn't start with a wildcard push first search term into sections and ser startsWithWildcard to false
		if (position != 0)
		{
			sections.push_back(searchSection(0));
			startsWithWildcard = false;
		}
		//loops until positions is more than string length
		while (position != std::string::npos)
		{
			//Push back current wildcard 
			sections.push_back(stringWildcard);
			//if the end of the word hasn't been reached, push the next search term into sections
			if (position != word.length() - 1)
				sections.push_back(searchSection(position));
			//position is set to the next wildcard position
			position = userInput.find(wildcard, position + 1);
		}
		//if the last entry in sections isn't a wildcard, set endsWithWildcard to false
		if (sections[sections.size() - 1] != stringWildcard)
			endsWithWildcard = false;
	}