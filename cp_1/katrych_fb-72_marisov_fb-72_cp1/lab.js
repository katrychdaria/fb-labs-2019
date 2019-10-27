var fs = require('fs');
var filter = require('./filter');
var { 
    letterFrequency, 
    bigramFrequencyWithoutCross,
    bigramFrequencyWithCross,
    monogramEntropy,
    bigramEntropy,
    printObject
} = require('./functions');

//with spaces
fs.appendFileSync('result_with_spaces.txt', 'Текст з пробілами\n\n');
var book = filter('book.txt');
var monograms = letterFrequency(book);
var bigramsWithCross = bigramFrequencyWithCross(book);
var bigramsWithoutCross = bigramFrequencyWithoutCross(book);
fs.appendFileSync('result_with_spaces.txt', `Ентропія
Ентропія монограм: ${monogramEntropy(monograms)}
Ентропія біграм з перетином: ${bigramEntropy(bigramsWithCross)}
Ентропія біграм без перетину: ${bigramEntropy(bigramsWithoutCross)}\n
`);
fs.appendFileSync('result_with_spaces.txt', 'Частота літер');
printObject(monograms, 'result_with_spaces.txt');
fs.appendFileSync('result_with_spaces.txt', 'Частота біграм з перетином');
printObject(bigramsWithCross, 'result_with_spaces.txt');
fs.appendFileSync('result_with_spaces.txt', 'Частота біграм без перетину');
printObject(bigramsWithoutCross, 'result_with_spaces.txt');

//without spaces
fs.appendFileSync('result_without_spaces.txt', 'Текст з пробілами\n\n');
var book = filter('book.txt').replace(/\s/g, '');
var monograms = letterFrequency(book);
var bigramsWithCross = bigramFrequencyWithCross(book);
var bigramsWithoutCross = bigramFrequencyWithoutCross(book);
fs.appendFileSync('result_without_spaces.txt', `Ентропія
Ентропія монограм: ${monogramEntropy(monograms)}
Ентропія біграм з перетином: ${bigramEntropy(bigramsWithCross)}
Ентропія біграм без перетину: ${bigramEntropy(bigramsWithoutCross)}\n
`);
fs.appendFileSync('result_without_spaces.txt', 'Частота літер');
printObject(monograms, 'result_without_spaces.txt');
fs.appendFileSync('result_without_spaces.txt', 'Частота біграм з перетином');
printObject(bigramsWithCross, 'result_without_spaces.txt');
fs.appendFileSync('result_without_spaces.txt', 'Частота біграм без перетину');
printObject(bigramsWithoutCross, 'result_without_spaces.txt');
