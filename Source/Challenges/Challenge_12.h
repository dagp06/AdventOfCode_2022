#pragma once

#include "Common/Challenge.h"

#include <set>
#include <queue>

using namespace std;

class CChallenge_12 : public CChallenge
{
private:
	struct Position
	{
		Position() {}
		Position(int _x, int _y) : x(_x), y(_y) {}

		bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
		bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }

		int x = 0;
		int y = 0;
	};

	struct Square
	{
		Square() {}
		Square(Position _pos, int _stepsTaken, int _heuristic) : m_Pos(_pos), m_StepsTaken(_stepsTaken), m_Heuristic(_heuristic) 
		{
			m_TotalEstimatedCost = m_StepsTaken + m_Heuristic;
		}

		bool operator==(const Square& _other) const { return m_Pos == _other.m_Pos; }
		bool operator<(const Square& _other) const { return m_TotalEstimatedCost < _other.m_TotalEstimatedCost; }

		Position m_Pos;
		int m_StepsTaken = 0;
		int m_Heuristic = 0;
		int m_TotalEstimatedCost = 0;
	};

	virtual EErrorCode SetUp_FirstPart() override;
	virtual EErrorCode Run_FirstPart() override;
	virtual EErrorCode CleanUp_FirstPart() override;

	virtual EErrorCode SetUp_SecondPart() override;
	virtual EErrorCode Run_SecondPart() override;
	virtual EErrorCode CleanUp_SecondPart() override;

	vector<Square>::iterator FindPos(vector<Square>& _squares, const Position& _pos);

#ifdef _DEBUG
	void PrintVisitedMap() const;
#endif

	// Part 1
	int Solve_1();
	void VisitNeighboors_1(const Square& _square, int& _stepsTakenCount);
	void GetVisitableNeighboors_1(const Position& _pos, vector<Position>& _outNeighboors);
	int ComputeHeuristic_1(const Position& _pos) const;


	// Part 2
	void Solve_2();
	void VisitNeighboors_2(const Square& _square);
	void GetVisitableNeighboors_2(const Position& _pos, vector<Position>& _outNeighboors);
	int ComputeHeuristic_2(const Position& _pos) const;
	int GetDistToEnd(const Position& _pos) const;
	void SetDistToEnd(const Position& _pos, int _value);

	static string const sm_inputFilePath;

	vector<vector<char>> m_HeightMap;
	Position m_EndPos;
	vector<Square> m_SquaresToVisit;

	// Part 1
	Position m_StartPos;
	vector<Square> m_VisitedSquares;

	// Part 2
	vector<vector<int>> m_DistToEndMap;
	Position m_BestStartPoint;

#ifdef _DEBUG
	vector<vector<bool>> m_DebugVisitedMap;
#endif
};
