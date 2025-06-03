import React, { useEffect, useState } from 'react';

function Board() {
  const [posts, setPosts] = useState([]);
  const [form, setForm] = useState({ title: '', content: '' });

  useEffect(() => {
    fetch('/api/post')
      .then(res => res.json())
      .then(data => setPosts(data));
  }, []);

  const submit = async (e) => {
    e.preventDefault();
    const res = await fetch('/api/post', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(form)
    });
    const newPost = await res.json();
    setPosts([...posts, newPost]);
    setForm({ title: '', content: '' });
  };

  return (
    <div>
      <h1>Board</h1>
      <form onSubmit={submit}>
        <input
          placeholder="Title"
          value={form.title}
          onChange={e => setForm({ ...form, title: e.target.value })}
        />
        <br />
        <textarea
          placeholder="Content"
          value={form.content}
          onChange={e => setForm({ ...form, content: e.target.value })}
        ></textarea>
        <br />
        <button type="submit">Write</button>
      </form>

      <hr />

      <ul>
        {posts.map(p => (
          <li key={p.id}>
            <strong>{p.title}</strong><br />
            {p.content}
          </li>
        ))}
      </ul>
    </div>
  );
}

export default Board;
