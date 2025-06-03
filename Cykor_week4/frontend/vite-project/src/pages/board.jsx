import React, { useEffect, useState } from 'react';

function Board() {
  const [posts, setPosts] = useState([]);
  const [form, setForm] = useState({ title: '', content: '' });
  const [edit_id, set_edit_id] = useState(null);

  useEffect(() => {
    fetch('/api/post')
      .then(res => res.json())
      .then(data => setPosts(data));
  }, []);

  const submit = async (e) => {
    e.preventDefault();

    if (edit_id === null) {
      const res = await fetch('/api/post', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(form)
      });
      const newPost = await res.json();
      setPosts([...posts, newPost]);
      setForm({ title: '', content: '' });
    }
    else {
      const res = await fetch(`/api/post/${edit_id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(form)
      });
      const updated = await res.json();
      setPosts(posts.map(p => p.id === edit_id ? updated : p));
      set_edit_id(null);
    }

    setForm({ title: '', content: '' });
  };

  const deletePost = async (id) => {
    await fetch(`/api/post/${id}`, {
      method: 'DELETE',
    });
    setPosts(posts.filter(p => p.id !== id));
  };

  const startEdit = (p) => {
    setForm({ title: p.title, content: p.content });
    set_edit_id(p.id);
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
        <br/>
        <textarea
          placeholder="Content"
          value={form.content}
          onChange={e => setForm({ ...form, content: e.target.value })}
        ></textarea>
        <br/>
        <button type="submit">{edit_id ? 'Update' : 'Write'}</button>
        {edit_id && (
          <button type="button" onClick={() => {
            set_edit_id(null);
            setForm({ title: '', content: '' });
          }}>Cancel</button>
        )}
      </form>

      <hr/>

      <ul>
        {posts.map(p => (
          <li key={p.id}>
            <strong>{p.title}</strong><br/>
            {p.content}<br/>
            <button onClick={() => deletePost(p.id)}>Delete</button>
            <button onClick={() => startEdit(p)}>Edit</button>
          </li>
        ))}
      </ul>
    </div>
  );
}

export default Board;
