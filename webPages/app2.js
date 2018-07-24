
function makeGoal (s) {
  var ts = s * s
  var puzzle = []
  for (var i = 0; i < ts; i++) {
    puzzle[i] = -1
  }
  var cur = 1
  var x = 0
  var ix = 1
  var y = 0
  var iy = 0

  while (true) {
    puzzle[x + y * s] = parseInt(cur)
    if (cur === 0) { console.log(puzzle); break }
    cur += 1
    if (x + ix === parseInt(s) || x + ix < 0 || (ix !== 0 && puzzle[x + ix + y * s] != -1)) {
      iy = ix
      ix = 0
    } else if (y + iy === parseInt(s) || y + iy < 0 || (iy !== 0 && puzzle[x + (y + iy) * s] != -1)) {
      ix = -iy
      iy = 0
    }
    x += ix
    y += iy
    if (cur === s * s) {
      cur = parseInt(0)
    }
  }
  console.log('puzzle=')
  console.log(puzzle)
  return (puzzle)
}

function getKeyByValue (object, value) {
  return Object.keys(object).find(key => object[key] === value)
}

function  getkey(c) {
  for(var key in c) {
    if(c[key] == 0) {
      return (key)
    }
  }
}

function makePuzzle (s, iterations) {
  if (parseInt(iterations) === 0) { iterations = 10000 }
  var p = makeGoal(s)
  function swapEmpty (p) {
    var idx = parseInt(getKeyByValue(p, 0))
    console.log('idx')
    console.log(idx)
    var poss = []
    if (idx % s > 0) { poss.push(idx - 1) }
    if (idx % s < s - 1) { poss.push(idx + 1) }
    if (idx / s > 0) { poss.push(idx - s) }
    if (idx / s < s - 1) { poss.push(idx + parseInt(s)) }
    var swi = poss[Math.round(Math.random() * poss.length)]
    p[idx] = p[swi]
    p[swi] = 0
  }
  for (var i = 0; i < iterations; i++) { swapEmpty(p) }
  return (p)
}

function takeForm (form) {
  var recive = {
    messageType: 0,
    data:
        {
          map: [],
          algorithm: 0,
          heuristicFunction: 0,
          solutionType: 0
        }
  }

  var elements = form.elements

  recive.data['map'] = makePuzzle(elements.side.value, elements.iterations.value)

  elements.group1.forEach(function (item, i, arr) {
    if (item.checked === true) {
      recive.data['heuristicFunction'] = i
    }
  })

  elements.group2.forEach(function (item, i, arr) {
    if (item.checked === true) {
      recive.data['algorithm'] = i
    }
  })

  console.log(JSON.stringify(recive))
  // recive.replace(/\\null/, 0)

  // recive = '{"messageType":"0","data":{"map":["11","0","9","4","2","15","7","1","13","3","12","5","8","6","10","14"],"algorithm":"0","heuristicFunction":"1","solutionType":"0"}}'

  recive = '{"messageType":0,"data":{"map":[1,2,3,8,0,4,7,6,5],"algorithm":0,"heuristicFunction":0,"solutionType":0}}'






  $.ajax({
    type: 'POST',
    url: '/message',
    data: recive,
    success: function (msg) {
      alert('Form Submitted: ' + msg)
    }
  })
}
