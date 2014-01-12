
#include <iostream>
#include <string>
#include <sstream>



using namespace std;

//function to evaluate is expression valid and call compute function if it is
// I used strings throughout to avoid the need to set a max expression size
bool evaluate(string expression, int&result);

//computes the value of an expression
int compute(string s);

//splits string in 2 based on a given delimiter
void string_split(string s, string &left, string &right, char delim);

//evaluates the expression within a set of brackets
string bracket_evaluate(string s);

int main(int argc, char *argv[]){

int result;
string expression;

cout << "Please enter the expression you wish to evaluate" << endl;
getline(cin,expression);

//this line removes whitespace from the expression (from stackoverflow)
expression.erase( std::remove_if( expression.begin(), expression.end(), ::isspace ), expression.end() );

if(evaluate(expression,result)){

	cout << "the expression you entered evaluated to :" << result << endl;

}else{
	cout << "The expression you entered contained an error" << endl;
}


} 


int compute(string s){
	//storage space for split string
	string left;
	string right;


	//scanning through the string for operators
	//operators are searched for in order of precedence
for(int i = 0; i < s.length(); i++)
{
	//brackets treated as special case here
	if(s[i] == '('){
		s = bracket_evaluate(s);
		return(compute(s));
	}

//for all other operators the string is split based on the operator called and then the 
//two resulting strings are used in a recursive call to the compute function
	if(s[i] == '+'){
		char delim = '+';
		string_split(s, left, right, delim);
		//the operator between the two recursive calls varies depending on which operator was detected
		return(compute(left)+compute(right));
	}

	if(s[i] == '-'){
		char delim = '-';
		string_split(s, left, right, delim);
		return(compute(left)-compute(right));
	}

	if(s[i] == '*'){
		char delim = '*';
		string_split(s, left, right, delim);
		return(compute(left)*compute(right));
	}

	if(s[i] == '/'){
		char delim = '/';
		string_split(s, left, right, delim);
		return(compute(left)/compute(right));
	}
}
//when no further operators are present the string must just be a number to it can be converted to an int
return atoi(s.c_str());
}

//splits the string based on a delimiter
void string_split(string s, string &left, string &right, char delim){
int index;
for(int i = 0; i < s.length(); i++){
	if(s[i] == delim){
		index = i;
		break;
	}
}
//using the c++ substr function to split the strings
left = s.substr(0,index);
right = s.substr(index+1,s.length());

}

//function used to compute expressions within brackets first
string bracket_evaluate(string s){
	//storage for variables used
string inner;
int LbracketIndex;
int RbracketIndex;
int result;
//boolean to detect nested brackets
bool first_outer = true;

//find the index of the two brackets within the string
for(int i = 0; i < s.length(); i++){
	if(s[i] == '(' && first_outer){
		LbracketIndex = i;
		// this ensures that the first ')' is located rather than the last
		first_outer = false;
	}
	//no boolean needed here since I want the furthest ')'
	if(s[i] == ')'){
		RbracketIndex = i;
	}
}
//extract the part of the string that will be removed later
string removed = s.substr(LbracketIndex,RbracketIndex-LbracketIndex+1);
//extract the expression within the brackets
inner = s.substr(LbracketIndex+1,RbracketIndex-LbracketIndex-1);
//evaluate the expression within the brackets only
result = compute(inner);

stringstream ss;
ss << result;
//convert the result back into a string
inner = ss.str();

//replace the original bracket expression with what it evaluates to (from stackoverflow)
size_t pos = s.find(removed);
if(pos != string::npos){
	s.replace(pos, removed.length(), inner);
}


//return this string to compute function
return s;

}



bool evaluate(string expression, int&result){

	int num_leftbracket = 0;
	int num_rightbracket = 0;

	//validate the expression that was input using the ascii values of the chars
	//note I could have used regexes here instead

	for(int i = 0; i < expression.length(); i++){
		if((expression[i] >= 47 && expression[i] <= 57) || (expression[i] >= 40 && expression[i] <= 43) || expression[i] == 45 || expression[i] ==32 ){
			if(expression[i] == 40){
				num_leftbracket++;
			}
			if(expression[i] == 41){
				num_rightbracket++;
			}
		}else{
			return false;
		}
	}
	if(num_leftbracket != num_rightbracket){
		return false;
	}

	//compute function is only called if the expression is valid
	result = compute(expression);

	return true;

}
