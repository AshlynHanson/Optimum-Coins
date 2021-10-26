/**
 * calculate and display an optimum set of coins to make an amount of
 * money
 * @author Ashlyn Hanson
 * @version March 7 2021
 */
#include <climits>
#include <iomanip>
#include <iostream>
#include <vector>
#include "matrix.h"
using namespace std;

/**
 * the recursive, memoized optimum function for computing the number
 * of coins to make an amount of money
 * @param i current index of denom vector
 * @param a current amount of money to make
 * @param memo the memo table
 * @param denom the coin denominations
 * @return the optimal number of coins from 0 to i to make a
 */
unsigned opt(size_t i, unsigned a, Matrix<unsigned>& memo,
             const vector<unsigned>& denom);

/**
 * use traceback to find out how many of each coin to use
 * @param memo the filled memo table
 * @param denom the coin denominations
 * @param amount the total amount of money needed
 */
void traceback(const Matrix<unsigned>& memo, const vector<unsigned>& denom,
               unsigned amount);

/**
 * dump the memo table to standard output
 * @param memo the memo table
 * @param denom the denominations of the coins
 */
void dump(const Matrix<unsigned> & memo, const vector<unsigned>& denom);

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " a d0 d1 d2 ..." << endl;
    cerr << "   where a is the amount to make and the d's are" << endl;
    cerr << "   coin denominations; d0 must be 1" << endl;
    return 1;
  }

  unsigned amount = static_cast<unsigned>(stoul(argv[1]));

  vector<unsigned> denom;

  for (unsigned index = 2; index < static_cast<unsigned>(argc); index++)
  {
    denom.push_back(static_cast<unsigned>(stoul(argv[index])));
  }

  Matrix<unsigned> memo(denom.size(), amount + 1);
  for (size_t coin = 0; coin < denom.size(); coin++)
  {
    for (unsigned a = 0; a <= amount; a++)
    {
      memo.at(coin, a) = UINT_MAX;
    }
  }

  unsigned number_of_coins = opt(denom.size() - 1, amount, memo, denom);

  cout << "The fewest coins to make " << amount <<
    " cents with these denominations is " << number_of_coins << endl;

  traceback(memo, denom, amount);

  cout << "The final memo table:" << endl;
  dump(memo, denom);
  return 0;
}

void dump(const Matrix<unsigned>& memo, const vector<unsigned>& denom )
{
  for (unsigned row = 0; row < memo.numrows(); row++)
  {
    cout << setw(3) << denom.at(row) << ": ";
    for (unsigned col = 0; col < memo.numcols(); col++)
    {
      if (memo.at(row, col) == UINT_MAX)
      {
        cout << "  -";
      }
      else
      {
        cout << setw(3) << memo.at(row, col);
      }
    }
    cout << endl;
  }
  cout << endl;
}

unsigned opt(size_t i, unsigned a, Matrix<unsigned>& memo,
             const vector<unsigned>& denom)
{
  if (memo.at(i, a) == UINT_MAX)
  {
    if (a == 0)
    {
      memo.at(i, a) = 0;
    }
    else if (i == 0)
    {
      memo.at(i, a) = opt(i, a - 1, memo, denom) + 1;
    }
    else if (a < denom.at(i))
    {
      memo.at(i, a) = opt(i - 1, a, memo, denom);
    }
    else
    {
      memo.at(i, a) = min(opt(i, a - denom.at(i), memo, denom) + 1,
        opt(i - 1, a, memo, denom));
    }
  }
  return memo.at(i, a);
}

void traceback(const Matrix<unsigned>& memo, const vector<unsigned>& denom,
               unsigned amount)
{
  size_t i = memo.numrows() - 1;
  size_t a = memo.numcols() - 1;
  unsigned new_amount = amount;
  unsigned current = memo.at(memo.numrows() - 1 , memo.numcols() - 1);

  while (a > 0)
  {
    if (i == 0)
    {
      new_amount -= denom.at(i);
      a = new_amount;
      current = memo.at(i, a);
      cout << "used coin: " << denom.at(i) << endl;
    }
    else if (memo.at(i - 1, a) == current)
    {
      i--;
      current = memo.at(i, a);
    }
    else
    {
      new_amount -= denom.at(i);
      a = new_amount;
      current = memo.at(i, a);
      cout << "used coin: " << denom.at(i) << endl;
    }
  }    
}

