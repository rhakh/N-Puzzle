void function(window, document, undefined) {
    var DEFAULT_SIDE = 3;
    var CELL_SIZE = 92;
    var CELL_GAP = 10;

    var totalMoves = 0;

    function Puzzle(container) {
        this.board = container;
        this.side = DEFAULT_SIDE;
        this.total = this.side * this.side;
        this.hole = this.total - 1;
        this.cells = [];
        this.finger = {};

        this.init();
    }
}(window, document);