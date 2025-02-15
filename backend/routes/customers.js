const express = require('express');
const router = express.Router();
const customers = require('../models/customers_model');


// Hakee kaikki asiakkaat
router.get('/', function (req, res) {
  customers.getAllCustomers(function (err, result) {
    if (err) {
      res.json(err);
    } else {
      res.json(result);
    }
  });
});

// Lisää uusi asiakas
router.post('/', function (req, res) {
  customers.addCustomer(req.body, function (err, result) {
    if (err) {
      res.json(err);
    }
    else {
      res.json(result);
    }
  });
});

// Päivittää asiakkaan ID:n perusteella
router.put('/:id', function (req, res) {
  customers.updateCustomer(req.params.id, req.body, function (err, result) {
    if (err) {
      res.json(err);
    }
    else {
      res.json(result);
    }
  });
});

// Poistaa asiakkaan ID:n perusteella
router.delete('/:id', function (req, res) {
  customers.deleteCustomer(req.params.id, function (err, result) {
    if (err) {
      res.json(err);
    }
    else {
      res.json(result);
    }
  });
});



router.get('/fullname/:id', function (req, res) {
  customers.getFullName(req.params.id, function (err, result) {
      if (err) {
          res.json(err);
      } else {
          res.json(result);
      }
  });
});

module.exports = router;
