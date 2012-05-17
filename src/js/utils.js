function inherits(C, P) {
    var F = function() {};
    F.prototype = P.prototype;
    C.prototype = new F();
    C.uber = P.prototype;
    C.prototype.constructor = C;
}

function mergeInto(A, B) {
    var prop;
    for (prop in B) {
        if (B.hasOwnProperty(prop)) {
            A[prop] = B[prop];
        }
    }
}
