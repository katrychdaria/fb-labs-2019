var fs = require('fs');

module.exports = {
    letterFrequency: function(book) {
        var i, obj = {}, letter;
        for (i = 0; i < book.length; i++) {
            if (obj[book[i]]) {
                obj[book[i]] += 1;
            } else {
                obj[book[i]] = 1;
            }
        }
        for (letter in obj) {
            obj[letter] = obj[letter] / book.length;
        }
        return Object
            .keys(obj)
            .sort((a, b) => obj[b] - obj[a])
            .reduce((sorted, i) => ({
              ...sorted, 
              [i]: obj[i]
            }), {});
    },

    bigramFrequencyWithoutCross: function(book) {
        var i, obj = {}, bigram;
        for (i = 0; i < book.length - 1; i += 2) {
            if (obj[book.slice(i, i + 2)]) {
                obj[book.slice(i, i + 2)] += 1;
            } else {
                obj[book.slice(i, i + 2)] = 1;
            }
        }
        for (bigram in obj) {
            obj[bigram] = obj[bigram] / Math.floor(book.length / 2);
        }
        return Object
            .keys(obj)
            .sort((a, b) => obj[b] - obj[a])
            .reduce((sorted, i) => ({
              ...sorted, 
              [i]: obj[i]
            }), {});
    },

    bigramFrequencyWithCross: function(book) {
        var i, obj = {}, bigram;
        for (i = 0; i < book.length - 1; i++) {
            if (obj[book.slice(i, i + 2)]) {
                obj[book.slice(i, i + 2)] += 1;
            } else {
                obj[book.slice(i, i + 2)] = 1;
            }
        }
        for (bigram in obj) {
            obj[bigram] = obj[bigram] / (book.length - 1);
        }
        return Object
            .keys(obj)
            .sort((a, b) => obj[b] - obj[a])
            .reduce((sorted, i) => ({
              ...sorted, 
              [i]: obj[i]
            }), {});
    },

    monogramEntropy: function(obj) {
        var letter, entropy = 0;
        for (letter in obj) {
            entropy -= obj[letter] * Math.log2(obj[letter]);
        }
        return entropy;
    },

    bigramEntropy: function(obj) {
        var bigram, entropy = 0;
        for (bigram in obj) {
            entropy -= obj[bigram] * Math.log2(obj[bigram]);
        }
        return entropy / 2;
    },

    printObject: function(obj, path) {
        fs.appendFileSync(path, `\n`);
        var i;
        for (i in obj) {
            fs.appendFileSync(path, `[${i}]: ${obj[i]}\n`);
        }
        fs.appendFileSync(path, `\n\n`);
    }
}