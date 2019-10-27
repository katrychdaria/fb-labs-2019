var fs = require('fs');

module.exports = function filter(book) {
    var bookText = fs.readFileSync(book, 'utf-8');
    bookText = bookText.replace(/ё|Ё/g, 'е');
    bookText = bookText.replace(/ъ|Ъ/g, 'ь');
    bookText = bookText.replace(/([^А-Яа-я ])+/g, " ");
    bookText = bookText.replace(/\s\s+/g, " ");
    bookText = bookText.toLowerCase();
    return bookText;
}

