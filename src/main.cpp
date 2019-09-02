//#include "../include/question_gen.hpp"
#include "fileHandler.cpp"
#include <stdlib.h>
#include <time.h>
#define MAXVAL 48

void questionGen(int, int, int, std::string);
int equation(std::string, int, int, int, int, int, int, int, int);
void solve(std::string, int, int, int, int, int, int);

int main()
{
  //initialise variables for number of easy, medium and hard algebra questions generated
  int easy, medium, hard;
  //obtains current directory
  std::string dir = getCurrentDir();

  //reads config for values. Has default if can't be read
  readConfig(&easy,&medium,&hard,dir);
  questionGen(easy,medium,hard,dir);
  return 0;
}

std::string questionNumb(int numb,bool questions)
{
  std::string numbstr = std::to_string(numb); std::string name;
  if (questions)
    name = numbstr + ". Questions.txt";
  else
    name = numbstr + ". Answers.txt";
  return name;
}

//method for generating questions
void questionGen(int easy, int medium, int hard, std::string dir)
{
  //creates file
  std::string questionsDIR = dir + "\\generated questions\\";
  //initialises filenumber
  short j = 1;
  //will name the file "j. Questions.txt" where j hasn't been used before
  while (testFile(questionsDIR, questionNumb(j,true), false) && j < 256)
    j++;
  //if the limit has been met, either something has went wrong or the folder has not been cleaned of previous questions
  if (j > 255)
  {
    std::cout << "\n\n-= WARNING =-\nEither an error has occured or you have reached the limit of 256 questions.\n"
              << "Please clean out the folder containing the questions to continue...\n";
    return;
  }
  //if able to use question number
  else
  {
    //generates random seed for number generator
    srand(time(NULL));
    //initialises coefficients and random gen number
    int coef1 = 0, coef3 = 0, random;
    //will do this for the total number of questions
    for (int i = 1; i < (easy+medium+hard+1); i++)
    {
      //if there are easy questions
      if (i < easy+1 && easy > 0)
      {
        //random number dictates the location of the x and if it is negative
        random = rand()%4;
        switch(random)
        {
          case 0:
            coef1 = 1;
            coef3 = 0;
            break;
          case 1:
            coef3 = 1;
            coef1 = 0;
            break;
          case 2:
            coef3 = -1;
            coef1 = 0;
            break;
          case 3:
            coef1 = -1;
            coef3 = 0;
            break;
        }
      }
      //if there are any medium questions
      else if (i < medium+1 && medium > 0)
      {
        //random value determines if the x is on the left or the right.
        random = rand()%2;
        if (random == 0)
        {
          coef1 = 0;
          //until a random number that is not 0 is chosen, keep choosing random number for x coefficient
          while(coef1 == 0)
            coef1 = rand()%MAXVAL - MAXVAL/2;
          coef3 = 0;
        }
        else
        {
          coef3 = 0;
          //until a random number that is not 0 is chosen, keep choosing random number for x coefficient
          while(coef3 == 0)
            coef3 = rand()%MAXVAL - MAXVAL/2;
          coef1 = 0;
        }
      }
      else if (i < hard+1 && hard > 0)
      {
        coef1 = 0;
        coef3 = 0;
        //chooses random number for the two x coefficients that are not the same or equation is trivial
        while(coef1 == coef3 || coef1 == 0 || coef3 == 0)
        {
          coef1 = rand()%MAXVAL - MAXVAL/2;
          coef3 = rand()%MAXVAL - MAXVAL/2;
        }

      }
      //a random value is chosen for the none x constant
      int coef2 = rand()%MAXVAL - MAXVAL/2;

      //the final coefficient is determined by the others. This is to ensure that there is a simple solution rather than a decimal
      int coef4 = equation(dir,j,i,coef1,coef2,coef3,easy,medium,hard);

      //method for solving
      solve(dir,j,i,coef1,coef2,coef3,coef4);
    }
    return;
  }
}

//function for creating an equation
int equation(std::string dir, int setNumber, int numb, int coef1, int coef2, int coef3, int easy, int medium, int hard)
{
  //creates and initialises file
  std::ofstream file;
  std::string questionsDIR = dir + "\\generated questions\\";
  //std::ofstream::app prevents clear and overwriting
  file.open((questionsDIR + questionNumb(setNumber,true)).c_str(),std::ofstream::app);

  //if the question number is 1, print the title
  if (numb == 1)
  {
    file<<"\nQuestions: \n\n\n";
    //if there is at least one easy question
    if (easy > 0)
      file<<"Easy:\n";
  }
  //print the title for medium or hard if they exist
  else if (numb == easy + 1 && medium > 0)
    file<<"\nMedium:\n";
  else if (numb == medium + easy + 1 && hard > 0)
    file<<"\nHard:\n";

  //prints the question number
  file<<numb<<".\t\t";

  /*
  this section of the code is entirally for formatting and standard mathematical formatting
  eg:               -1x + 0 = +3 + 2x
  would be shown as
                    -x = 3 + 2x
  This is just for presentation purposes
  */

  //determines the formatting for x coefficients. For example, 1x will be shown as "x" and 0x will not be shown
  switch (coef1){
    case 1:
      file<<"x ";
      break;
    case -1:
      file<<"-x ";
      break;
    case 0:
      break;
    default:
      file<<coef1<<"x ";
      break;
  }
  //determines the formatting for the first none x coefficient
  if (coef2 < 0)
  {
    if (coef1 == 0)
      file<<coef2<<" ";
    else
      file<<"- "<<-coef2<<" ";
  }
  else if (coef2 > 0)
  {
    if (coef1 != 0)
      file<<"+ ";
    file<<coef2<<" ";
  }
  if (coef1 == 0 && coef2 == 0)
    file<<0<<" ";

  //prints equals sign
  file<<"= ";

  //determines the formatting for x coefficient. For example, 1x will be shown as "x" and 0x will not be shown
  switch (coef3)
  {
    case 1:
      file<<"x ";
      break;
    case -1:
      file<<"-x ";
      break;
    case 0:
      break;
    default:
      file<<coef3<<"x ";
      break;
  }

  //as the second none x coefficient still needs to be calculated:
  int coef4 = 1;
  //if coef1 and coef3 are trivial, coef4 can be anything
  if (coef1-coef3 == 0)
    coef4 = rand()%24;

  //if it is a none trivial solution
  else
  {
    //check to see if the difference of the none x coefficients are divisible by the x coefficients
    while(((coef4-coef2)%(coef1-coef3)) != 0)
    {
      bool check = false;
      //will generate a random number for coef4 until this is met and is not the same number as coef2
      coef4 = rand()%24 - 12;
      if (coef2 == coef4 && check == false)
      {
        check = true;
        coef4 = 1;
        continue;
      }
      //once a suitable number has been found, break and keep the value
      else
        break;
    }
  }

  //formatting for the new none x coefficient
  if (coef4 > 0)
  {
    if (coef3 != 0)
      file<<"+ ";
    file<<coef4;
  }
  else if (coef4 < 0)
  {
    if (coef3 == 0)
      file<<coef4;
    else
      file<<"- "<<-coef4;
  }
  //if the right hand side of the equation is equal to 0, the number 0 can be printed
  if (coef3 == 0 && coef4 == 0)
    file<<0;
  file<<"\n\n";
  file.close();

  //returns the generated value
  return coef4;
}

void solve(std::string dir, int setNumber, int numb, int coef1, int coef2, int coef3, int coef4)
{
  //new file
  std::string ansDIR = dir + "\\generated questions\\";
  std::ofstream file;
  //using std::ofstream::app prevents clear and overwrite
  file.open((ansDIR + questionNumb(setNumber,false)).c_str(),std::ofstream::app);

  //title
  if (numb == 1)
    file<<"\nAnswers:\n";

  //question number
  file<<"\n\n"<<numb<<".\t\t";

  //prints first coefficient
  if (coef1 > 1 || coef1 < -1)
    file<<coef1<<"x ";
  else if (coef1 == -1)
    file<<"-x ";
  else if (coef1 == 1)
    file<<"x ";
  //prints "+" if 2nd coefficient is positive and 1st exists
  if (coef2 >= 0)
  {
    if (coef1 != 0)
      file<<"+ ";
  }

  //prints 2nd coefficient
  file<<coef2<<" = ";

  //checks for 3rd coefficient
  if (coef3 != 0)
  {
    if (coef3*coef3 != 1)
      file<<coef3;
    else if (coef3 == -1)
      file<<"-";
    file<<"x ";
  }

  //prints 4th coefficient
  if (coef4 >= 0)
  {
    if (coef3 != 0)
      file<<"+ ";
  }

  //prints 4th coefficient
  file<<coef4<<"\n";

  //calculations used
  int coef5 = coef4 - coef2;
  int coef6 = coef1 - coef3;
  int coef7 = coef5/coef6;

  //////////////////////////////////////////////////////////////////////////////
  ////visual////
  //checks for 2nd coef
  int moves = 1;
  if (coef2 != 0)
  {
    if (coef1 == 0)
    {
      file<<"\nFirst, flip the equation so that the x's are on the left hand side\n\t";
      int tmp1 = coef1, tmp2 = coef2, tmp3 = coef3, tmp4 = coef4;
      coef1 = tmp3; coef2 = tmp4; coef3 = tmp1; coef4 = tmp2; moves++;

      //prints first coefficient
      if (coef1 > 1 || coef1 < -1)
        file<<coef1<<"x ";
      else if (coef1 == -1)
        file<<"-x ";
      else if (coef1 == 1)
        file<<"x ";

      //prints "+" if 2nd coefficient is positive and 1st exists
      if (coef2 >= 0)
      {
        if (coef1 != 0)
          file<<"+ ";
      }

      //prints 2nd coefficient
      file<<coef2<<" = ";

      //checks for 3rd coefficient
      if (coef3 != 0)
      {
        if (coef3*coef3 != 1)
          file<<coef3;
        else if (coef3 == -1)
          file<<"-";
        file<<"x ";
      }

      //prints 4th coefficient
      if (coef4 >= 0)
      {
        if (coef3 != 0)
          file<<"+ ";
      }
      //prints 4th coefficient
      file<<coef4<<"\n\n";

      //calculations used
      int coef5 = coef4 - coef2;
      int coef6 = tmp3 - tmp1;
      int coef7 = coef5/coef6;
    }

    if (coef1 != 0)
    {
      if (moves == 2)
        file<<"Next";
      else
        file<<"\nFirstly";
      file<<", move the numbers (anything that isn't x) from the left hand side of the equation to the right\n\t";
      moves++;
      if (coef1*coef1 != 1)
        file<<coef1;
      else if (coef1 == -1)
        file<<"-";
      file<<"x ";
    }

    //checks if coef2 is positive
    if (coef2 > 0)
    {
      if (coef1 != 0)
        file<<"+ ";
    }

    //displays subtraction of coef2
    file<<coef2<<" - ("<<coef2<<") = ";

    //checks for coef3
    if (coef3 != 0)
    {
      if (coef3*coef3 != 1)
        file<<coef3;
      else if (coef3 == -1)
        file<<"-";
      file<<"x ";
    }
    //checks if coef4 is positive
    if (coef4 >= 0)
    {
      if (coef3 != 0)
        file<<"+ ";
    }
    file<<coef4<<" - ("<<coef2<<")\n\t";
    ////////////////////////////////////////
    ////part 2////
    //checks for coef1
    if (coef1 != 0)
    {
      if (coef1*coef1 != 1)
        file<<coef1;
      else if (coef1 == -1)
        file<<"-";
      file<<"x ";
    }
    file<<"= ";
    //checks for 3rd coef
    if (coef3 != 0)
    {
      if (coef3* coef3 != 1)
        file<<coef3;
      else if (coef3 == -1)
        file<<"-";
      file<<"x ";
    }
    //checks new 4th coefficient
    if (coef5 >= 0)
    {
      if (coef3 != 0)
        file<<"+ ";
    }
    file<<coef5<<"\n";
  }
  //checks 3rd coef
  if (coef3 != 0)
  {
    if (moves > 1)
      file<<"\nNext";
    else
      file<<"\nFirst";
    file<<", move all the x's to one side of the equation\n\t";
    if (coef1 != 0)
    {
      if (coef1* coef1 != 1)
        file<<coef1;
      else if (coef1 == -1)
        file<<"-";
      file<<"x ";
    }
    file<<"- (";
    if (coef3 * coef3 != 1)
      file<<coef3;
    else if (coef3 == -1)
      file<<"-";
    file<<"x) = ";
    if (coef3 * coef3 != 1)
      file<<coef3;
    else if (coef3 == -1)
      file<<"-";
    file<<"x - (";
    if (coef3 * coef3 != 1)
      file<<coef3;
    else if (coef3 == -1)
      file<<"-";
    file<<"x) ";
    if (coef5 > 0)
      file<<"+ ";
    file<<coef5<<"\n";
    if (coef6 != 1)
      file<<coef6;
    file<<"x = "<<coef5<<"\n";
    if (coef6 == 1)
    {
      file<<"\n";
      return;
    }
  }
  //do if coef isnt 1
  if (coef6 != 1)
  {
    if (moves > 1)
      file<<"\nFinally, ";
    file<<"divide both sides by the number in front of the x\n\t";
    file<<coef6<<"x/"<<coef6<<" = "<<coef5<<"/"<<coef6<<"\n\t";
    file<<"x = "<<coef7<<"\n";
  }
  file.close();
}
