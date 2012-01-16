load("glycam.js");
load("09.js"); // charge defined here

var substructure = [ { at: "02", code: "4YB" }, "0VB" ];

var glycan = build([
  "ROH",
  "4YB",
  { at: "O4", code: "4YB" },
  { id: "one", at: "O4", code: "VMB" }, [
    { id: "two", at: "O3", code: "YMA" }, [
      substructure
    ],
    { at: "O4", code: "0YB" }
  ],
  { at: "O6", code: "2MA" },
  substructure
]);

var totalCharge = charge(glycan);
print("charge: ", totalCharge)
