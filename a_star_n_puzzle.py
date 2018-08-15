import heapq
import numpy as np
import time

def flatten(board):
    # if it's nested lists, flatten them. I do this with list comprehension taking each tile at a time from each sublist
    if type(board[1])==list:
        board = [item for sublist in board for item in sublist]
    # else, it should be a list of ints or floats
    elif type(board[1])==int or type(board[1])==float:
        board = board
    # if it's neither, it's a wrong input and will raise an error.
    else:
        raise ValueError("Class 'PuzzleNode' got values that are not a sublist of ints nor a flat list of ints.")
    return board

"""
Class: PuzzleNode
Purpose: Object for each puzzle board created during search
Arg: None
Class Functions
    __hash__(): return a hash value for the puzzle board to id the puzzle

    __str__(): return a matrix representation of the puzzle board in string format

    __eq__(others): return True if another PuzzleNode object is identical

    get_moves(): return PuzzleNodes object that are the possible moves
                    of a puzzle

    list_of_list(): transform the 1d array representation of the puzzle
                    into a multi-d array representation

    verify_input: verify whether the puzzle is n*n, and has all the numbers
                  in the board
"""
class PuzzleNode():
    def __init__(self, n, values, cost, parent, heuristic):

        #parent of the candidate puzzle
        self.parent = parent

        #dimension of the puzzle
        self.n = n
        #value of the initial puzzle pieces, stored as a 1d array
        self.tiles = flatten(values)
        self.tiles=values

        # To reconstruct the optimal solution, we need to store all the steps
        # To easily access each of the puzzles we have gone through, we store the hash value associated with each puzzle
        self.puzzleid = hash(tuple(self.tiles))

    #Hash the puzzle to have the puzzleid, or just return the puzzleid if there is one
    def __hash__(self):
        if self.puzzleid is None:
            self.puzzleid = hash(tuple(self.tiles))
        return self.puzzleid

    #Print out a grid of the board state
    def __str__(self):
        #To print a grid, it is easier to convert the 1d board array to text first
        strings_list = [str(x) for x in self.tiles]
        #Create n rows and n columns
        rows = [" ".join(strings_list[i:i + self.n]) for i in xrange(0, self.n**2, self.n)]
        #Break the rows into different lines
        return "\n".join(rows)

    # For checking if 2 candidate puzzles are equal
    def __eq__(self, other):
        return self.tiles == other.tiles

    #For getting possible moves from the current state
    def get_moves(self):
        #get the index of where the 0 is
        zeroPos = self.tiles.index(0)
        n = self.n
        candidates = []

        #Swap appropriate tiles with the 0 tile
        def swap(zeroPos,move,n):
            temp = list(self.tiles)
            swapPos = zeroPos + move
            #Evaluating edge cases
            if zeroPos%n == 0 and move == -1:
                return
            elif zeroPos%n == n-1 and move == 1:
                return
            elif zeroPos/n == 0 and move == -n:
                return
            elif zeroPos/n == n-1 and move == n:
                return
            else: #Swap tiles and create new PuzzleNode object to store the new board
                temp[zeroPos],temp[swapPos] = temp[swapPos],temp[zeroPos]
                result = PuzzleNode(self.n,temp,0,self.puzzleid,None)
            return result

        #Generate at most 4 candidate boards from the current board
        if swap(zeroPos,1,n) is not None:
            #print "+1 is added"
            yield swap(zeroPos,1,n)
        if swap(zeroPos,-1,n) is not None:
            #print "-1 is added"
            yield swap(zeroPos,-1,n)
        if swap(zeroPos,n,n) is not None:
            #print "+n is added"
            yield swap(zeroPos,n,n)
        if swap(zeroPos,-n,n) is not None:
            #print "-n is added"
            yield swap(zeroPos,-n,n)

    #transform the tiles again from 1d array to list of lists
    def list_of_list(self):
        return [self.tiles[i:i+self.n] for i in xrange(0, self.n**2, self.n)]

    #verify the validity of the initial board
    def verify_input(self):
        err = 0
        reason = "Input was valid"
        initial_state = self.tiles
        n = self.n

        #Check the dimension of the puzzle
        if n<2 or n>=128:
            err = -1
            reason = "Puzzle size not valid"

        #Check if the puzzle has the size of n^2
        if len(initial_state) != n*n:
            err = -1
            reason = "Puzzle size is not n^2"

        sorted_list = sorted(initial_state)
        verified_list = range(n**2)
        #Compare the puzzle list with all numbers from 0 to n^2-1
        if sorted_list != verified_list:
            err = -1
            reason = "Puzzle does not contain all numbers from 0 to n^2-1"

        #break the program when there is an error
        if err == -1:
            raise ValueError(reason)

        return err, reason, initial_state

"""
Function isSolvable

Purpose: Determine whether a given board is solvable based on
         inversion rule

Arg:
    board: (list) a list_of_list representation of the board configuration

Return:
    err: (int) -2 if the board is unsolvable; 0 if the board is solvable
    reason: (str) the reason for the error code
"""
def isSolvable(board):
    inversions = 0
    n = int(len(board)**0.5)
    zeroPos = board.index(0)
    for i in xrange(len(board)):
        for j in xrange(i+1,len(board)):
            if board[i] > board[j] and board[j] != 0:
                inversions += 1

    if n%2 == 0: #grid width is even
        if (zeroPos/n)%2 == 0: #0 tile on even row counting from bottom
            if inversions%2 == 1: #inversions is odd is solvable
                err = 0
                reason = "The puzzle is solvable"
            else:
                err = -2
                reason = "The puzzle's width is even, 0 tile on even row counting from bottom, inversions is even. Puzzle unsolvable"
        else: #0 tile on odd row counting from bottom
            if inversions%2 == 0: #inversions is even is solvable
                err = 0
                reason = "The puzzle is solvable"
            else:
                err = -2
                reason = "The puzzle's width is even, 0 tile on odd row counting from bottom, inversions is odd. Puzzle unsolvable"

    else: #grid width is odd
        if inversions%2 == 0:
            err = 0
            reason = "The puzzle is solvable"
        else:
            err = -2
            reason = "The puzzle's width is odd, and the inversions is odd. Puzzle unsolvable"

    if err == -2:
        raise ValueError(reason)
    return err, reason

"""
Function: solvePuzzle

Purpose: Using A* search with heuristic to solve a n^n puzzle

Arg:
    n: (int) dimension of the puzzle
    state: (list) the initial puzzle board
    heuristic: (function) the heuristic function used in the A* search
    prnt: (boolean) whether or not to print the full solution

Return:
    steps: (int) number of search steps before solving the puzzle
    frontierSize: (int) largest frontier size during search
    err: (int) 0 means no error; 1 means the puzzle is invalid
    run_time: (time) the time needed to solve the puzzle
"""
def solvePuzzle (n, state, heuristic, prnt=False):

    start_time = time.time()
    run_time = 0.0

    queue = [] #priority queue to determine the least costly node to search
    total_cost = {} #total cost of the shortest path
    heuristic_cost = {} #cache of previous heuristic cost of boards
    visited = {} #the puzzle boards expanded and searched
    steps_to_sol = [] #detailed steps towards solution
    frontierSize = 0 #largest frontier size of the search tree
    steps = -1 #number of steps needed to solve

    tiles = flatten(state) #1d representation of the puzzle

    #Defining current state and goal state
    start = PuzzleNode(n, tiles, 0, None, heuristic)
    goal = PuzzleNode(n, range(n**2),100,None,heuristic)

    #verify whether the intial puzzle board is valid
    err, reason, initial_state = start.verify_input()
    #using isSolvable() to check whether the initial puzzle is solvable
    if err == 0:
        err, reason = isSolvable(start.tiles)
    unsolved = True

    #Initializing heap and total cost
    heapq.heappush(queue,(0,start))
    total_cost[start] = 0

    if prnt:
        if heuristic == manhattanDistance:
            print "Solving using Manhattan Distance...\n"
        elif heuristic == misplacedTiles:
            print "Solving using Misplaced Tiles...\n"
        print "Start solving puzzle from:\n"
        print "{}\n".format(start.__str__())

    #traverse through all the candidates until there is none
    while unsolved:
        steps += 1
        #Select the least costly node to expand using priority queue
        cost, current = heapq.heappop(queue)
        current_cost = total_cost[current]
        #Put the searched puzzle board into the visited store
        visited[current] = current

        #When the current board matches with the goal board
        if current.tiles == goal.tiles:
            unsolved = False
            if prnt:
                print "Puzzle Solved!\n"
                print "Initial Puzzle Board:\n"
                print "{}\n".format(start.__str__())
                print "Final Puzzle Board:\n"
                print "{}\n".format(current.__str__())
            run_time = time.time()-start_time
            break

        if prnt:
            print "Currently inspecting...\n"
            print "{}\n".format(current.__str__())

        #Evaluate every candidate move
        candidates = current.get_moves()
        for move in candidates:
            """For debugging
            print "Inspecting Candidates...\n"
            print "Evaluating this candidate:\n{}".format(move)
            print "Steps now: {}\n".format(steps)
            """
            #Prevent searching an already searched puzzle board
            if move not in total_cost or total_cost[move] > current_cost + 1:
                total_cost[move] = current_cost + 1
                #Add the unaccounted heuristic cost into the cache
                if move not in heuristic_cost:
                    #update the total cost of the move
                    total_cost[move] += heuristic(move.list_of_list())

                #Push the path back to the priority queue
                heapq.heappush(queue,(total_cost[move],move))

            #Update the frontierSize
            frontierSize = max(frontierSize,len(queue))

    if prnt:
        #printing the number of steps and frontier size of the solution
        print "Number of steps:", steps
        print "Frontier size:", frontierSize
        print "Error: {} ({})".format(err,reason)
        print "Runtime: {}\n".format(run_time)
        #printing all the steps leading to the solution
        print "Steps to solve the puzzle (in reverse order)..."
        for searched_steps in visited:
            print "{};".format(visited[searched_steps].list_of_list())

    return steps, frontierSize, err, run_time

"""
Function: manhattanDistance

Purpose: One of the heuristics to solve the N-puzzle problem.
         Calculate the manhattan distance of any given board
        (the number of moves needed to transform any given board
         to a complete board)

Arg:
    board: (list) a list-of-lists representation of the puzzle board
    n: (int) the dimension of the board

Return:
    manDis: (int) the total manhattan distance of a given puzzle board

**Auxiliary function: manhattanDistance_per_tile(tiles,i,n)
Purpose: calculate the manhattan distance of a given tile in the board
Arg:
    tiles: (int) the numeric value of the tile
    i: (int) the position of the tile (array index of the board array)
    n: (int) dimension of the given board
Return:
    steps: (int) manhattan distance of the given tile in the given puzzle
"""
def manhattanDistance(board):
    #Convert the board back to 1d array for easy manipulation
    n = len(board[0])
    tiles = flatten(board)
    manDis = 0
    #Sum the manhattanDistance of all the tiles
    for i in tiles:
        manDis += manhattanDistance_per_tile(tiles[i],i,n)
    return manDis

def manhattanDistance_per_tile(tiles,i,n):
    goalPos = tiles
    currentPos = i
    steps = 0
    #Perform when the tile is not at its place
    while currentPos != goalPos:
        #Shift levels when the current position of the tile is not at the same level
        if currentPos/n != goalPos/n:
            if currentPos > goalPos:
                currentPos -= n
            else:
                currentPos += n
            steps += 1
        #Move left or right depending on where the tile needs to go
        else:
            if currentPos > goalPos:
                currentPos -= 1
            else:
                currentPos += 1
            steps += 1
    return steps

"""
Function: misplaceTiles

Purpose: One of the heuristics for the N-puzzle problem.
         Calculate the number of misplaced tiles in a given puzzle board

Arg:
    board: (list) a list-of-lists representation of a given puzzle board

Return:
    misplace: (int) number of misplaced tiles in the given puzzle board
"""
def misplacedTiles(board):
    tiles = flatten(board)
    misplace = 0
    for i in tiles:
        if tiles[i] != i:
            misplace += 1
    return misplace

"""
Function: linearConflict

Purpose: Calculate the Manhattan Distance the board, accounting for steps
         needed to bypass linear conflicts

Arg:
    board: (list) the board given to calculate Manhattan Distance

Return:
    total_moves: (int) The Manhattan Distance, accounting for steps
    needed to bypass linear conflicts

Auxiliary Function: linearConflict_per_tile(tiles,i,n)
Arg: tiles - the puzzle board; i: the position of the tile currently
     examining; n: dimension of the board
Return: Manhattan Distance of the individual tile (accounting for
        linear conflict)
"""
def linearConflict(board):
    n = len(board[0])
    tiles = flatten(board)
    total_moves = 0
    #Sum the manhattanDistance of all the tiles
    for i in xrange(len(tiles)):
        total_moves += linearConflict_per_tile(tiles,i,n)
    return total_moves

def linearConflict_per_tile(tiles,i,n):
    goalPos = tiles[i]
    currentPos = i
    steps = 0
    #Perform when the tile is not at its place
    while currentPos != goalPos and tiles!=0:
        #Shift levels when the current position of the tile is not at the same level
        if currentPos/n != goalPos/n:
            if currentPos > goalPos:
                currentPos -= n
            else:
                currentPos += n
            steps += 1
        #Move left or right depending on where the tile needs to go
        else:
            if currentPos > goalPos:
                currentPos -= 1
            else:
                currentPos += 1
            steps += 1

    #inspect conflict
    currentRow = currentPos/n
    nextRow = (currentPos+1)/n
    if currentRow == nextRow and i%n != n-1:
        currentGoalRow = goalPos/n
        nextGoalRow = tiles[i+1]/n
        if currentGoalRow == nextGoalRow and tiles[currentPos] > tiles[currentPos+1]:
            steps += 2
    return steps

"""
Function: nMaxSwap

Purpose: Calculate the number of direct swaps with 0 tile to solve
         the n-puzzle

Arg:
    board: (list) a list-of-list representation of the board

Return:
    swaps: (int) number of direct swaps needed to solve the given puzzle
"""
def nMaxSwap(board):
    n = len(board[0])
    tiles = flatten(board)
    swaps = 0
    solved = [0 for i in range(len(tiles))]
    while tiles != range(n**2):
        zeroPos = tiles.index(0)
        if zeroPos != 0:
            swapPos = tiles.index(zeroPos)
            tiles[zeroPos],tiles[swapPos] = tiles[swapPos],tiles[zeroPos]
            solved[zeroPos] = 1
            swaps += 1
        else:
            count = 1
            while solved[count] == 1 or tiles[count] == count:
                count += 1
                continue
            swapPos = count
            tiles[zeroPos],tiles[swapPos] = tiles[swapPos],tiles[zeroPos]
            swaps += 1
    return swaps

#heuristics as a list of functions
heuristics = [manhattanDistance,misplacedTiles,nMaxSwap,linearConflict]

#Used to compare different heuristics
def test_heuristic(pset):
    for boards in pset:
        print "Solving this board: {}\n".format(boards)
        stepsMan, frontierSizeMan, errMan, runtimeMan = solvePuzzle(len(boards[0]),boards,heuristics[0])
        stepsMis, frontierSizeMis, errMis, runtimeMis = solvePuzzle(len(boards[0]),boards,heuristics[2])
        print "\t        Manhattan Distance  vs  nMaxSwap "
        print "Steps:          \t    {} \t  {}".format(stepsMan,stepsMis)
        print "Frontier size:  \t    {} \t  {}".format(frontierSizeMan,frontierSizeMis)
        print "Runtime (sec):  \t    {0:.3f} \t  {1:.3f}".format(runtimeMan,runtimeMis)
        print "\n=======================================================\n"

test1 = [[5,7,6],[2,4,3],[8,1,0]]
test2 = [[7,0,8],[4,6,1],[5,3,2]]
test3 = [[2,3,7],[1,8,0],[6,5,4]]
unsolvable_test1 = [[2,1,0],[3,4,5],[6,7,8]]
pset = [test1,test2,test3]

test_heuristic(pset)
