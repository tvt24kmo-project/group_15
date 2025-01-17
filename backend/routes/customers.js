const express = require('express');
const router = express.Router();
const customers = require('../models/customers_model');



router.get('/', function (req, res) {
  customers.getAllCustomers(function (err, result) {
    if (err) {
      res.json(err);
    } else {
      res.json(result);
    }
  });
});


router.get('/:fname', function (req, res) {
  customers.getByfname(req.params.fname, function (err, result) {
    if (err) {
      res.json(err);
    } else {
      res.json(result);
    }
  });
});


module.exports = router;
