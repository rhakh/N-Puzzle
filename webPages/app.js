
void (function (window, document, undefined) {
  var DEFAULT_SIDE = 3
  var CELL_SIZE = 92
  var CELL_GAP = 10

  var totalMoves = 0

  function Puzzle (container, size) {
    this.board = container
    this.side = size
    this.total = this.side * this.side
    this.hole = this.total - 1
    this.cells = []
    this.finger = {}

    // this.init();
    this.fillCells()
  }

  Puzzle.prototype.placeCell = function (cell, i) {
    cell.style.left = ((i % this.side) * (CELL_SIZE + CELL_GAP) + CELL_GAP) + 'px'
    cell.style.top = (Math.floor(i / this.side) * (CELL_SIZE + CELL_GAP) + CELL_GAP) + 'px'
  }

  Puzzle.prototype.fillCells = function () {
    var fragment = document.createDocumentFragment()
    for (var i = 0; i < this.total - 1; i++) {
      var cell = document.createElement('span')
      cell.className = 'cell'
      cell.innerHTML = i + 1
      this.cells[i] = i + 1
      this.placeCell(cell, i)
      fragment.appendChild(cell)
    }
    this.cells.push(0)
    this.goal = this.cells.slice()
    this.board.appendChild(fragment)
    this.board.className = 'board'
    this.board.style.height = this.board.style.width = (this.side * (CELL_SIZE + CELL_GAP) + CELL_GAP) + 'px'
  }

  Puzzle.prototype.init = function () {
    this.fillCells()
    // this.defineActions();
    // this.shuffleCells();
  }

  window.Puzzle = Puzzle

}(window, document))
