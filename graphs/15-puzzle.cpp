#include <iostream>
#include<array>
#include<queue>
#include<algorithm>
#include<cassert>
#include<string>
#include<unordered_map>
#include<memory>
#include<set>
const char PuzzleSize = 16;
const char RowSize = 4;

//std::array<char, PuzzleSize> FinishField = { 1,2,3,4,5,6,7,8,0 };
std::array<char, PuzzleSize> FinishField = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };


class GameState {
public:

	GameState(const std::array<char, PuzzleSize> &_field);
	GameState(const GameState& state);

	bool CanMoveLeft() const;
	bool CanMoveRight()const;
	bool CanMoveUp() const;
	bool CanMoveDown() const;
	bool isComplete() const;
	GameState MoveLeft() const;
	GameState MoveRight()const;
	GameState MoveUp() const;
	GameState MoveDown() const;
	int diffFromFinish()const;
	bool operator==(const GameState & state)const;
	bool operator<(const GameState & state)const;
	friend std::ostream& operator<<(std::ostream& stream, const GameState& state);
	friend struct StateHash;
private:
	std::array<char, PuzzleSize> field;
	char emptyPos;
};

GameState::GameState(const std::array<char, PuzzleSize> &_field) :field(_field)
{
	emptyPos = -1;
	for (int i = 0; i < PuzzleSize; i++) {
		if (field[i] == 0) {
			emptyPos = i;
		}
	}
	assert(emptyPos != -1);
}

int GameState::diffFromFinish()const {
	int counter = 0;
	for (int i = 0; i < PuzzleSize; i++) {
		if (field[i] == 0)
			continue;
		counter +=4* (
			abs(
				i % RowSize - (field[i] - 1) % RowSize
				) +
			abs(
				i / RowSize - (field[i] - 1) / RowSize
				)
			);
	}
	return counter;
}

//Hasher for unordered_map
struct StateHash {
	size_t operator()(const GameState& state) const {
		size_t hash = 0;
		//memcpy(&hash, &(state.field[0]), sizeof(hash));
		//return hash;
		for (auto e : state.field) {
			hash ^= std::hash<int>{}(e)+0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
	}


};

bool GameState::operator<(const GameState & state)const {
	const StateHash hash1;
	const StateHash hash2;
	size_t hash_1 = hash1(*this);
	size_t hash_2 = hash2(state);
	return hash_1 < hash_2;
}

std::ostream&   operator<<(std::ostream& stream, const GameState& state) {
	for (int i = 0; i < RowSize; i++) {
		for (int j = 0; j < RowSize; j++) {
			stream << static_cast<int>(state.field[i * RowSize + j]) << ' ';
		}
		stream << '\n';
	}
	return stream;
}
bool GameState::operator==(const GameState& state) const {
	return field == state.field;
}
bool GameState::isComplete() const {
	return field == FinishField;
}
GameState::GameState(const GameState& state) :
	field(state.field), emptyPos(state.emptyPos) {

}
bool GameState::CanMoveLeft() const {
	return emptyPos % RowSize != (RowSize - 1);
}
bool GameState::CanMoveRight()const {
	return emptyPos % RowSize != 0;
}
bool GameState::CanMoveUp() const {
	return emptyPos < (PuzzleSize - RowSize);
}
bool GameState::CanMoveDown() const {
	return emptyPos > (RowSize - 1);
}

GameState GameState::MoveLeft()const {
	assert(CanMoveLeft());
	GameState leftstate(*this);
	std::swap(leftstate.field[emptyPos], leftstate.field[emptyPos + 1]);
	leftstate.emptyPos++;
	return leftstate;

}
GameState GameState::MoveRight()const {
	assert(CanMoveRight());
	GameState leftstate(*this);
	std::swap(leftstate.field[emptyPos], leftstate.field[emptyPos - 1]);
	leftstate.emptyPos--;
	return leftstate;
}
GameState GameState::MoveUp()const {
	assert(CanMoveUp());
	GameState upstate(*this);
	std::swap(upstate.field[emptyPos], upstate.field[emptyPos + RowSize]);
	upstate.emptyPos += RowSize;
	return upstate;
}
GameState GameState::MoveDown()const {
	assert(CanMoveDown());
	GameState downstate(*this);
	std::swap(downstate.field[emptyPos], downstate.field[emptyPos - RowSize]);
	downstate.emptyPos -= RowSize;
	return downstate;
}



std::string GetSolution(const std::array<char, PuzzleSize> & field) {
	std::unordered_map<GameState, char, StateHash> visited;
	std::unordered_map<GameState, char, StateHash> distanceToStart;
	std::unordered_map<GameState, GameState, StateHash> pred;

	GameState startState(field);
	GameState finishState(FinishField);

	visited[startState] = 'S';

	// enque
	// first-> Vertex distance second->vertex itself
	//Qu stores "state common weigth" and "State object" pairs
	std::set<std::pair<int, const GameState>> qu;
	//weigth is "distance to be passed back to start" +"  difference between finish state and vertex itself"

	distanceToStart[startState] = 0;
	auto temp = std::make_pair((distanceToStart[startState] + startState.diffFromFinish()), startState);
	qu.insert(temp);
	//auto iter = (qu.begin());
	//auto current = *iter;
	//qu.erase(iter);

	int visit_counter = 1;
	while (true) {
		//Get State with best heuristic estimation
		auto iter = (qu.begin());
		int current_state_weight = (iter->first);
		GameState current_state = (iter->second);
		qu.erase(iter);
		if (current_state.isComplete()) break;

		//Check next possible unvisited vertexes of current vertex
		{
			//MOVE LEFT	
			if (current_state.CanMoveLeft()) {
				GameState leftState = current_state.MoveLeft();
				if (visited.find(leftState) == visited.end()) {
					//if not visited yet push to queue
					visited[leftState] = 'L';
					visit_counter++;
					distanceToStart[leftState] = distanceToStart[current_state] + 1;
					qu.insert(std::make_pair(leftState.diffFromFinish() + distanceToStart[leftState], leftState));

				}

			}
			//MOVE RIGHT
			if (current_state.CanMoveRight()) {
				GameState rightState = current_state.MoveRight();
				if (visited.find(rightState) == visited.end()) {
					visited[rightState] = 'R';
					visit_counter++;
					distanceToStart[rightState] = distanceToStart[current_state] + 1;

					qu.insert(std::make_pair(rightState.diffFromFinish() + distanceToStart[rightState], rightState));
				}

			}
			//MOVE UP
			if (current_state.CanMoveUp()) {
				GameState upState = current_state.MoveUp();
				if (visited.find(upState) == visited.end()) {
					visited[upState] = 'U';
					visit_counter++;
					distanceToStart[upState] = distanceToStart[current_state] + 1;

					qu.insert(std::make_pair(upState.diffFromFinish() + distanceToStart[upState], upState));
				}

			}
			//MOVE DOWN
			if (current_state.CanMoveDown()) {
				GameState downState = current_state.MoveDown();
				if (visited.find(downState) == visited.end()) {
					visited[downState] = 'D';
					visit_counter++;
					distanceToStart[downState] = distanceToStart[current_state] + 1;

					qu.insert(std::make_pair(downState.diffFromFinish() + distanceToStart[downState], downState));
				}

			}
		}
	}
	//std::cout << visit_counter << "\n";

	//End of routing
	std::string path;
	//Get path to final state
	GameState state(finishState);
	//std::cout << state << '\n';
	int hodov = 0;
	while (visited[state] != 'S') {
		switch (visited[state]) {
		case 'L':
			state = state.MoveRight();
			path += 'L';
			hodov++;
			break;
		case 'R':
			state = state.MoveLeft();
			path += 'R';
			hodov++;
			break;
		case 'U':

			state = state.MoveDown();
			path += 'U';
			hodov++;
			break;

		case 'D':

			state = state.MoveUp();
			path += 'D';
			hodov++;
			break;
		default:
			assert(false);

		}
		//std::cout << state << '\n';
	}
	std::cout << hodov << "\n";
	std::reverse(path.begin(), path.end());
	return path;
}

// Check if puzzle is solvable(16 or 9 positions)
bool isSolveable(const std::array<char, PuzzleSize>& field)
{
	int parity = 0;
	int row = 0;
	int blankRow = 0;
	for (int i = 0; i < PuzzleSize; i++)
	{
		if (i % RowSize == 0) {
			row++;
		}
		if (field[i] == 0) {
			blankRow = row;
			continue;
		}
		for (int j = i + 1; j < PuzzleSize; j++)
		{
			if (field[i] > field[j] && field[j] != 0)
			{
				parity++;
			}
		}
	}

	if (RowSize % 2 == 0) {
		if (blankRow % 2 == 0) {
			return parity % 2 == 0;
		}
		else {
			return parity % 2 != 0;
		}
	}
	else {
		return parity % 2 == 0;
	}
}


//TODO count inversions to cheak if puzzle solvable
int main() {
//	std::array<char, PuzzleSize> field = { 2,3,1,4,5,6,7,8,0 };

	//std::array<char, PuzzleSize> field = { 10,4,3,5,7,6,2,8,9,0,11,13,1,12,14,15 };

	std::array<char, PuzzleSize> field;
	for (int i = 0; i < 16; i++) {
		int a;
		std::cin >> a;
		field[i] = (char)a;
	}
	if (!isSolveable(field)) {
		std::cout << -1 << '\n';
		//system("pause");
		return 0;
	}
	std::cout << GetSolution(field) << '\n';
	//system("pause");
	return 0;
}

