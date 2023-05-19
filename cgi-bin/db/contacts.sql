CREATE TABLE contacts ( 
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(500) NOT NULL,
  email VARCHAR(500),
  phone VARCHAR(500),
  address VARCHAR(500),
  city VARCHAR(500),
  province VARCHAR(500),
  message VARCHAR(1000)
);