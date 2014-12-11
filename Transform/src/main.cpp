// Problem: Transform a number into it's palindrome if the palinrdome is less than 10E9 
// Author: Sourabh Bodas

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

class Transform 
{
	public:
		Transform();
		int palindrome (int N);
		bool ifPalindrome (int num);
		~Transform();
	
	private:
		
		// Actual one used for inverting the number
		inline int invertNumDigits(int num);
	
		// Implemented for alternative method but not used in this code
		inline std::vector<int> numToVec (int num);
		inline int vecToNum (std::vector<int> vec);
};

Transform::Transform()
{
	
}

Transform::~Transform()
{
	
}

inline std::vector<int> Transform::numToVec (int num)
{
	int numCopy = num;
	std::vector<int> numArr;
	int i = 0;

	do
	{	
		numArr.push_back(numCopy%10);
		
		numCopy = numCopy / 10;
		
		//std::cout << numArr[i] << "\n";	
	 	
		i++;
	}	
	while(numCopy > 0);
	
	return numArr;
}

inline int Transform::vecToNum(std::vector<int> vec)
{
	std::vector<int> numArr = vec;
	int i = 0;
	int digit = 0;
	int num = 0;
	do
	{	
		digit = int(vec[i]);
		digit = digit * pow(10,i);
		
		num = num + digit;
		
		i++;
	}	
	while(int(numArr.size()) >= i);
	
	return num;
	
}

inline int Transform::invertNumDigits(int num)
{
    int invNum = 0;
    while(num > 0)
    {
            invNum = invNum*10 + (num % 10);
            num = num/10;
    }
    //cout << invNum << endl;
	
	return invNum;
}

int Transform::palindrome (int N)
{
	// Check if the number is palindrome in itself
	if(ifPalindrome (N))
	   return N;
	   
	// Declare the variables required
	int numCopy = N;
	long int palNum = 0;
	long int tempNum = 0;
	
	// Invert the digits for input number
	int invNum = invertNumDigits (numCopy);	
	
	// Add the number and inverted number
	tempNum = numCopy + invNum;
	//std::cout << tempNum << std::endl;	
	
	
	// Check if resulting sum is palindrome
	if(ifPalindrome (tempNum))
		return tempNum;
	
	// If not call the function recursively if the sum is less than 10E9
	else if (tempNum <= 1000000000)
		palindrome(tempNum);
	
	// If the sum is more than 10E9 then output -1
	else
	//palNum = -1;
		return palNum = -1;
	
}

bool Transform::ifPalindrome (int num)
{
	std::string s = std::to_string(num);

    if( equal(s.begin(), s.begin() + s.size()/2, s.rbegin()) )
        return true;
    else
        return false;
}

int main()
{
	int inNum;
	Transform transform;
	std::cout << "This program returns Palindrome of a number, please enter a number between 1 to 10000" << std::endl;
	std::cin >> inNum;
	
	// Check if input number is less than 10000 and positive, if not return -1
	if(inNum > 10000 || inNum < 1)
	{
		std::cout << "Sorry the number is out of specified range" << std::endl;
		return -1;
	}
	
	// Call the Palindrome method
	long int palNum = transform.palindrome(inNum);
	
	// Display the Palindrome of a number
	std::cout << "Palindrome constructed using number " << inNum << " is " << palNum << std::endl;
	return 0;
	
}