
void function(window, document, undefined) {
    var DEFAULT_SIDE = 3;
    var CELL_SIZE = 92;
    var CELL_GAP = 10;

    var totalMoves = 0;


    function Puzzle(container, size) {
        this.board = container;
        this.side = size;
        this.total = this.side * this.side;
        this.hole = this.total - 1;
        this.cells = [];
        this.finger = {};

        // this.init();
        this.fillCells();
    }

    Puzzle.prototype.placeCell = function(cell, i) {
        cell.style.left = ((i % this.side) * (CELL_SIZE + CELL_GAP) + CELL_GAP) + "px";
        cell.style.top = (Math.floor(i / this.side) * (CELL_SIZE + CELL_GAP) + CELL_GAP) + "px";
    };

    Puzzle.prototype.fillCells = function() {
        var fragment = document.createDocumentFragment();
        for(var i = 0; i < this.total - 1; i++) {
            var cell = document.createElement("span");
            cell.className = "cell";
            cell.innerHTML = i + 1;
            this.cells[i] = i + 1;
            this.placeCell(cell, i);
            fragment.appendChild(cell);
        }
        this.cells.push(0);
        this.goal = this.cells.slice();
        this.board.appendChild(fragment);
        this.board.className = "board";
        this.board.style.height = this.board.style.width = (this.side * (CELL_SIZE + CELL_GAP) + CELL_GAP) + "px";
    };

    Puzzle.prototype.init = function() {
        this.fillCells();
        // this.defineActions();
        // this.shuffleCells();
    };

    window.Puzzle = Puzzle;







    function makeGoal(s) {
        let ts = s * s;
        puzzle = [];
        for (let i = 0; i < ts; i++) {
            puzzle[i] = -1;
        }
        cur = 1;
        x = 0;
        ix = 1;
        y = 0;
        iy = 0;

        while (true) {
            puzzle[x + y*s] = cur;
            if (cur === 0)
                break;
            cur += 1;
            if (x + ix === s || x + ix < 0 || (ix !== 0 && puzzle[x + ix + y*s] !== -1)) {
                iy = ix;
                ix = 0;
            }
            else if (y + iy === s || y + iy < 0 || (iy !== 0 && puzzle[x + (y+iy)*s] !== -1)) {
                ix = -iy;
                iy = 0;
            }
            x += ix;
            y += iy;
            if (cur === s * s) {
                cur = 0;
            }
        }
        console.log(puzzle);
        return(puzzle);
    }

}(window, document);
