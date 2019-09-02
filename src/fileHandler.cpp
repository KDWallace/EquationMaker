#include "../include/question_gen.hpp"

bool testFile(std::string);

//method for reading config file and assigning number of each type of question
void readConfig(int* easy, int* medium, int* hard, std::string dir)
{
  std::string configDIR = dir + "\\config files\\";
  if (!testFile(configDIR, "config.txt", true))
  {
    std::cout << "\nAttempting to write to config.txt...\n";
    writeConfig(configDIR,15,10,5);
    *easy = 15;
    *medium = 10;
    *hard = 5;
    std::cout << "Config.txt reset successfully!";
  }
  //if the file can be found first time, the contents of the file will be tested to see if they're valid input
  else
  {
    std::ifstream config;
    config.open((dir + "\\config files\\config.txt").c_str());
    std::cout << "\n-= The config.txt file was found! =-\nChecking file...\n";
    //temp string for checking lines
    std::string tmp;
    //booleans to check if the types of questions have been found yet
    bool checkEasy = false; bool checkMed = false; bool checkHard = false;

    //will check every line word for word for numbers
    while (getline(config,tmp,' '))
    {
      std::istringstream getNum(tmp);

      //will check if the number of easy questions has been found, if not:
      if (!checkEasy)
      {
        //will attempt to assign value to easy
        getNum >> *easy;

        //if unable to assign due to it being a string, it will clear the buffer and move on
        if (getNum.fail() && !getNum.eof())
          getNum.clear();
        //if successfully assigned to number, the check can be made to true
        else
        {
          if (*easy < 0)
            *easy = 0;
          checkEasy = true;
        }
      }

      //will check if the number of medium questions has been found, if not:
      else if (!checkMed)
      {
        //will attempt to assign value to medium
        getNum >> *medium;

        //if unable to assign due to it being a string, it will clear the buffer and move on
        if (getNum.fail() && !getNum.eof())
          getNum.clear();

        //if successfully assigned to number, the check can be made to true
        else
        {
          if (*medium < 0)
            *medium = 0;
          checkMed = true;
        }
      }

      //will check if the number of hard questions has been found, if not:
      else if (!checkHard)
      {
        //will attempt to assign value to hard
        getNum >> *hard;

        //if unable to assign due to it being a string, it will clear the buffer and move on
        if (getNum.fail() && !getNum.eof())
          getNum.clear();

        //if successfully assigned to number, the check can be made to true
        else
        {
          if (*hard < 0)
            *hard = 0;
          checkHard = true;
        }
      }
    }
    //closes the config file
    config.close();

    //if easy wasn't found then no number was found in the file, therefore the file cannot be read and will be recreated
    if (!checkEasy)
    {
      std::cout << "\n\nThere appears to be something wrong with the config.txt file.\nAttempting to fix by deleting it and replacing with default options...\n";

      //if the program is unable to delete the file, it must be open somewhere
      if (remove((configDIR + "config.txt").c_str()) != 0)
        std::cout << "\nPlease ensure that the config.txt file is closed so it can be re-created\n";

      //if the program can delete the file, it will use this method to do so
      else
      {
        std::cout << "Re-creating config.txt...\n";
        readConfig(easy,medium,hard,dir);
      }
      return;
    }

    //if medium was not found, assume that the value is 0
    if (!checkMed)
    {
      std::cout << "Number of medium questions not found, assuming none wanted\n";
      *medium = 0;
    }

    //if hard was not foung, assume that the value is 0
    if (!checkHard)
    {
      std::cout << "Number of hard questions not found, assuming none wanted\n";
      *hard = 0;
    }
    //rewrites the config with the read values to ensure the legibility of the file for future users
    //this is to prevent users writing to the config in a way that makes it hard to understand, yet can be still understood by the program
    writeConfig(configDIR, *easy, *medium, *hard);
    return;
  }
}

//function for obtaining current directory to be used when obtaining config and for creating questions
std::string getCurrentDir()
{
  //initialise new buffer of max filename size
  char buffer[FILENAME_MAX];

  //get the curernt working directory and assign to buffer
  getcwd(buffer,FILENAME_MAX);

  //convert to string and return
  std::string cwd(buffer);
  return cwd;
}

//method for creating and writing to the config file using the directory given
void writeConfig(std::string configDIR, int easy, int medium, int hard)
{
  //assigns the int values of the number of questions to string form
  std::string easystr = std::to_string(easy);
  std::string mediumstr = std::to_string(medium);
  std::string hardstr = std::to_string(hard);

  //creates the file in the given directory
  std::ofstream config;
  config.open((configDIR + "config.txt").c_str());

  //writes to the file the information as well as the number of each type of question
  config << "Configuration file for the maths questions program!\nHere you'll be able to modify what questions will be asked.\n";
  config << "Just change the number next to the question type.\nNote that the program will read the numbers in order in the file.\n\n";
  config << "Easy questions = " + easystr;
  config << "\nMedium questions = " + mediumstr;
  config << "\nHard questions = " + hardstr;

  //closes the file
  config.close();
  return;
}

bool testFile(std::string dir, std::string filename, bool shouldExist)
{
  //will attempt to open the file in the directory
  std::ifstream file;
  file.open((dir + filename).c_str());

  //if the file cannot be opened but should already exist (e.g. config.txt)
  if (!file)
  {
    //if the file should exist (eg. config.txt), it will inform the user
    if (shouldExist)
      std::cout << filename <<" is missing. Creating the missing files in " + dir + "\n";
    file.close();

    //attempts to create the file in the expected directory
    std::ofstream newfile;
    newfile.open((dir + filename).c_str());

    //if the file cannot be created, this is most likely due to the directory being missing. Will attempt to create a new one
    if (!newfile)
    {
      std::cout << "The folder appears to be missing, creating new folder...\n";
      newfile.close();
      try
      {
          mkdir(dir.c_str());
          return false;
      }
      //upon failing to create the directory, the program will continue with the default values of easy = 15, medium = 10 and hard = 5
      catch (int e)
      {
        //if the directory cannot be made, will return true to stop the program from attempting to create the file
        std::cout << "An error has occured while trying to recreate the missing folders.\n";
        return true;
      }

    }
    //if the folder can be made, the file will return false as it does not exist yet
    else
    {
      newfile.close();
      return false;
    }
  }
  else
  {
    file.close();
    return true;
  }
}
