#include "pch.h"
#include "wordsearch.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "WORD SEARCH PROGRAM" << endl;
	//the file is passed the current directory of the .exe to construct a path to the text file
	File myFile(argv[0]);
	//the path being searched is printed to the console
	cout << "Searching: " << myFile.getPath() << endl;
	//if the file can be opened, continue with the rest of the program
	if (myFile.openFile())
	{
		//set the text in the text file to a string value in myFile
		string line;
		while (getline(myFile.file, line))
		{		
			myFile.appendFileContent(line);
		}
		//the file content is printed to the console and then the file is closed
		cout << "File Content: " << myFile.getFileContent() << endl;
		myFile.closeFile();
		string userStringInput;
		string searchString = "";
		cout << "This string search is case sensitive, if you wish to use a wildcard use the '*' character" << endl;
		bool valid;
		//loop until the user's input is valid
		do {

			cout << "Please enter the string you wish to search for: ";
			cin >> userStringInput;
			
			//if the string is longer than one character 
			if (userStringInput.length() != 1)
			{
				//loop until the end of the string -1
				for (size_t loop = 0; loop != userStringInput.length() - 1; loop++)
				{
					//if there isn't a wildcard at the current position and a wildcard in the next position then add current position to new string
					if (!(userStringInput.at(loop) == '*' && userStringInput.at(loop + 1) == '*'))
						searchString += userStringInput.at(loop);
					//if the loop is about to terminate, add the final character to the string
					if (loop == userStringInput.length() - 2)
						searchString += userStringInput.at(loop + 1);
				}
			}else
				//if there is only 1 character in the string, set it to the new string
				searchString = userStringInput;

			//if the string is just a wildcard, the inform the user it can't just be a wildcard and set valid to false
			if (searchString == "*")
			{
				cout << "Please enter a valid input - Cannot enter a wildcard (*) by itself" << endl;
				valid = false;
			}
			else
				valid = true;
		} while(!valid);
		//searchString is passed to search to be segmented
		SearchWord search(searchString);
		vector<int> searchPositions;
		vector<int>::iterator pos;
		vector<TextEntry> entriesFound;
		int loopStart;
		//if the search string starts with a wildcard, search for all positions from the first text entry (sections[1])
		if (search.startsWithWildcard == true)
		{
			myFile.searchStringForAllPositions(search.sections[1], searchPositions);
			loopStart = 1;
		}
		//if the search string does not start with a wildcard, search for all positions from the first text entry (sections[0])
		else
		{
			myFile.searchStringForAllPositions(search.sections[0], searchPositions);
			loopStart = 0;
		}
		//for every position found, create an entry
		for (pos = searchPositions.begin(); pos != searchPositions.end(); ++pos) 
		{
			myFile.createEntry(entriesFound, *pos, search.sections, search.startsWithWildcard, search.endsWithWildcard, searchPositions, loopStart);
		}
		cout << "The entries found in the text file:" << endl;
		//loop and output every entry that isn't a repeat
		for (size_t loop = 0; loop != entriesFound.size(); ++loop) {
			entriesFound[loop].getEntry();
		}
	}
	//Pause the program before terminating
	system("PAUSE");
	return 0;
}
