#ifndef ORegexParse_h__
#define ORegexParse_h__
#include "NFAState.h"

//! NFA/DFA Table
typedef std::deque<NFAState*> FSA_TABLE;

class ORegexParse
{
private:
	int m_nNextStateID;	
	//! Operator Stack
	/*! Operators are simple characters like "*" & "|" */
	std::stack<char> m_OperatorStack;

	//! Operand Stack
	/*! If we use the Thompson's Algorithm then we realize
		that each operand is a NFA automata on its own!
	*/
	std::stack<FSA_TABLE> m_OperandStack;
		/*! NFA Table is stored in a deque of CAG_States.
		Each CAG_State object has a multimap of 
		transitions where the key is the input
		character and values are the references to
		states to which it transfers.
	*/
	FSA_TABLE m_NFATable;
	FSA_TABLE m_DFATable;
public:
	ORegexParse();


	int NFAToDFA();

	/// 将一个正则表达式转换为NFA
	/// 其中用到2个算法：1个为堆栈方法求值
	/// http://blog.csdn.net/liuhuiyi/article/details/8433203
	///  Algorithm Thompson's construction
	/// http://www.cppblog.com/woaidongmao/archive/2010/10/21/97245.html
	bool CreateNFA(std::string strRegEx);

private:
	///将两个字符之间省略的连接符添加上
	std::string ConcatExpand(std::string strRegEx);
	

	int NFAStackPop(FSA_TABLE &NFATable);
	int NFAStackPush(FSA_TABLE &NFATable);

	///将一个字符，装换为一个状态合集，放入operand stack	
	int PushOneByte(char chInput);



	//! Evaluates the concatenation operator
	/*! This function pops two operands from the stack 
		and evaluates the concatenation on them, pushing
		the result back on the stack.
	*/
	bool Concat();

	//! Evaluates the Kleen's closure - star operator
	/*! Pops one operator from the stack and evaluates
		the star operator on it. It pushes the result
		on the operand stack again.
	*/
	bool Star();

	//! Evaluates the union operator
	/*! Pops 2 operands from the stack and evaluates
		the union operator pushing the result on the
		operand stack.
	*/
	bool Union();

//////////////////////////////////////////////////////////////////////////

	//! Checks is a specific character and operator
	bool IsOperator(char ch) { 
		return ((ch == OPERATOR_STAR) || (ch == OPERATOR_UNION) ||
			(ch == OPERATOR_LEFTP) || (ch == OPERATOR_RIGHTP) || (ch == OPERATOR_CONCAT)); };
	
	//! Checks if the specific character is input character
	bool IsInput(char ch) { return(!IsOperator(ch)); };
	
	/// 左括号
	//! Checks is a character left parantheses
	bool IsLeftParanthesis(char ch) { return(ch == OPERATOR_LEFTP); };
	
	/// 右括号
	//! Checks is a character right parantheses
	bool IsRightParanthesis(char ch) { return(ch == OPERATOR_RIGHTP); };


	//! Evaluates the next operator from the operator stack
	bool Eval();


	/// 查看优先级，如果opLeft<=opRight 则返回1，否则返回0
	//! Returns operator presedence
	/*! Returns 1 if presedence of opLeft <= opRight.
	
			Kleens Closure	- highest ()
			Concatenation	- middle  ab
			Union			- lowest  |
	*/
	bool Presedence(char opLeft, char opRight)
	{
		if(opLeft == opRight)
			return 1;

		if(opLeft == OPERATOR_STAR)/// star优先级最高
			return 0;

		if(opRight == OPERATOR_STAR)
			return 1;

		if(opLeft == OPERATOR_CONCAT)///优先级中等
			return 0;

		if(opRight == OPERATOR_CONCAT)
			return 1;

		if(opLeft == OPERATOR_UNION)
			return 0;
		
		return 1;
	};

	/// mC=0时，为eps的跳转
	std::set<NFAState*> MoveOne(char mC, std::set<NFAState*> mNFAs);
	//所有通过eps能相互连在一起的
	std::set<NFAState*> MoveZero(std::set<NFAState*> mNFAs, std::set<NFAState*> &mRes1);

    /// 检查mA在mDFAs中是否存在,如果存在，则返回已存在的指针，否则返回0
    NFAState* GetExistState(NFAState* mA, FSA_TABLE &mDFAs);
};

#endif // ORegexParse_h__

