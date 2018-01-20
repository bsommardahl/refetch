'use strict';

var Rebase = require("@glennsl/rebase/src/Rebase.bs.js");

var btoa = (
  function () {
    if (btoa) return btoa;

    var Buffer = Buffer || require('buffer').Buffer;
    return function (str) {
      return new Buffer(str).toString('base64');
    }
  }()
);

function reduceOr($$default, f, param) {
  if (param) {
    return Rebase.List[/* reduce */3](f, param[0], param[1]);
  } else {
    return $$default;
  }
}

var List = /* module */[/* reduceOr */reduceOr];

exports.btoa = btoa;
exports.List = List;
/* btoa Not a pure module */