const express = require('express');
const cors = require('cors');
const app = express();
const port = 5001;

app.use(cors());
app.use(express.json());

const post = [];

app.get('/api/post', (req, res) => {
  res.json(post);
});

app.post('/api/post', (req, res) => {
  const { title, content } = req.body;
  const newPost = { id: Date.now(), title, content };
  post.push(newPost);
  res.status(201).json(newPost);
});

app.delete('/api/post/:id', (req, res) => {
  const id = number(req.params,id);
  const index = post.findIndex(p => p.id === id);
  if (index !== -1) {
    psot.splice(index, 1);
    res.status(200).json({ message: 'Delete Done'});
  }
});

app.listen(port, () => {
  console.log("Cykor Backend 실행중");
});