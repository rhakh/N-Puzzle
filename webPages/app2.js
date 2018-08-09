const CELL_SIZE = 92;
const CELL_GAP = 10;

let totalMoves = 0;

function Puzzle(container, elements, json) {
  this.board = container;
  $('.board').empty();
  this.initialState = json.data.map;
  this.side = elements.side.value;
  this.total = parseInt(this.side * this.side);
  this.hole = parseInt(this.total - 1);
  this.cells = [];

  // this.init();
  this.fillCells(json.data.map);
}

Puzzle.prototype.swapCells = function (direction) {
  let target = -1;
  switch (direction) {
    case '3':
      target = parseInt(this.hole) - 1;
      break;
    case '2': // down
      // if (this.hole < this.total - this.side) {
      target = parseInt(this.hole) + parseInt(this.side);
      // }
      break;
    case '4': // right
      // if (this.hole % this.side > 0) {
      target = this.hole + 1;
      // }
      break;
    case '1': // up
      // if (this.hole > this.side - 1) {
      target = this.hole - this.side;
      // }
      break;
  }

  if (target > -1) {
    this.moveCell(target);
  }
};

Puzzle.prototype.moveCell = function (target) {
  var cell = document.getElementsByClassName('cell')[5];
  for (let i = 0; i < this.total; i++) {
    var cell = document.getElementsByClassName('cell')[i];
    if (parseInt(cell.innerHTML) === this.cells[target]) { break; }
  }
  const hole = document.getElementsByClassName('cell')[this.hole];
  console.log(`hole is -${hole.innerHTML}`);
  console.log(`inner is - ${cell.innerHTML}`);
  console.log(`selected is - ${cell.innerHTML}`);
  this.placeCell(cell, this.hole);
  this.cells[this.hole] = this.cells[target];
  this.cells[target] = 0;
  this.hole = target;
  totalMoves++;
  console.log(`target was - ${target}`);
};

Puzzle.prototype.placeCell = function (cell, i) {
  cell.style.left = `${(i % this.side) * (CELL_SIZE + CELL_GAP) + CELL_GAP}px`;
  cell.style.top = `${Math.floor(i / this.side) * (CELL_SIZE + CELL_GAP) + CELL_GAP}px`;
};

Puzzle.prototype.fillCells = function (map) {
  console.log(map);

  const fragment = document.createDocumentFragment();
  for (let i = 0; i < this.total; i++) {
    if (parseInt(map[i]) === 0) {
      var cell = document.createElement('span');
      this.hole = i;
      cell.innerHTML = map[i];
      cell.className = 'cell';
      cell.style = 'display:none';
      cell.id = i;
      this.cells.push(0);
      this.placeCell(cell, i);
      fragment.appendChild(cell);
      continue;
    }
    cell = document.createElement('span');
    cell.id = i;
    cell.className = 'cell';
    cell.innerHTML = map[i];
    this.cells[i] = map[i];
    this.placeCell(cell, i);
    fragment.appendChild(cell);
  }
  this.goal = this.cells.slice();
  this.board.appendChild(fragment);
  this.board.className = 'board';
  this.board.style.height = this.board.style.width = `${this.side * (CELL_SIZE + CELL_GAP) + CELL_GAP}px`;
};

function makeGoal(s) {
  const ts = s * s;
  const puzzle = [];
  for (let i = 0; i < ts; i++) {
    puzzle[i] = -1;
  }
  let cur = 1;
  let x = 0;
  let ix = 1;
  let y = 0;
  let iy = 0;
  while (true) {
    puzzle[x + y * s] = parseInt(cur);
    if (cur === 0) { break; }
    cur += 1;
    if (x + ix === parseInt(s) || x + ix < 0 || (ix !== 0 && puzzle[x + ix + y * s] != -1)) {
      iy = ix;
      ix = 0;
    } else if (y + iy === parseInt(s) || y + iy < 0 || (iy !== 0 && puzzle[x + (y + iy) * s] != -1)) {
      ix = -iy;
      iy = 0;
    }
    x += ix;
    y += iy;
    if (cur === s * s) {
      cur = parseInt(0);
    }
  }
  return (puzzle);
}

function gK(obj) {
  let i = 0;
  const len = obj.length;
  while (i < len) {
    if (obj[i] == 0) {
      return (i);
    }
    i++;
  }
}

function makePuzzle(s, iterations) {
  if (parseInt(iterations) === 0) { iterations = 100; }
  if (parseInt(iterations) === -1) { iterations = 0; }
  const p = makeGoal(s);
  function swapEmpty(p) {
    const idx = parseInt(gK(p));
    const poss = [];
    if (idx % s > 0) { poss.push(idx - 1); }
    if (idx % s < s - 1) { poss.push(idx + 1); }
    if (Math.floor(idx / s) > 0) { poss.push(idx - s); }
    if (Math.floor(idx / s) < s - 1) { poss.push(idx + parseInt(s)); }
    const rand = Math.round(Math.random() * (poss.length - 1));
    const swi = poss[rand];
    p[idx] = p[swi];
    p[swi] = 0;
  }
  for (let i = 0; i < iterations; i++) {
    swapEmpty(p);
  }
  console.log(p);
  return (p);
}

function handleResponse(moves, puzzle) {
  console.log(`moves is + ${moves}`);
  for (let i = 1; i < moves.length; i++) {
    puzzle.swapCells(moves[i]);
    console.log('--------------------------------------');
  }
}

let puzzle;
function takeForm(form, puzzle) {
  const recive = {
    messageType: 0,
    data:
        {
          map: [],
          algorithm: 0,
          heuristicFunction: 0,
          solutionType: 0,
        },
  };

  const elements = form.elements;

  recive.data.map = makePuzzle(elements.side.value, elements.iterations.value);
  console.log(`recieved = ${recive.data.map}`);
  elements.group1.forEach((item, i, arr) => {
    if (item.checked === true) {
      recive.data.heuristicFunction = i;
    }
  });

  elements.group2.forEach((item, i, arr) => {
    if (item.checked === true) {
      recive.data.algorithm = i;
    }
  });

  JSON.stringify(recive);
  puzzle = new Puzzle(document.getElementById('puzzle'), elements, recive);

  $.ajax({
    type: 'POST',
    url: '/message',
    data: JSON.stringify(recive),
    success(msg) {
      msg = JSON.parse(msg);
      const moves = msg.data.movements;
      console.log(moves);
      if (moves !== undefined) { handleResponse(msg.data.movements, puzzle); }
    },
  });
}
