CREATE TABLE users (
    username VARCHAR(50) NOT NULL PRIMARY KEY,
    password VARCHAR(50) NOT NULL
);

CREATE TABLE posts (
    title VARCHAR(100),
    content TEXT,
    writer VARCHAR(50)
);
