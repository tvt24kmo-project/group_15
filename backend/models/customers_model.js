const db = require('../database');

const customers = {
    getAllCustomers: function (callback) {
        return db.query("SELECT * FROM customers", callback);
    },

    getByfname: function (fname, callback) {
        return db.query("select * from customers where fname=?", [fname], callback);
    },

    getByLname: function (lname, callback) {
        return db.quert("select * from customers where lname=?", [lname], callback);
    }

};

module.exports = customers;
