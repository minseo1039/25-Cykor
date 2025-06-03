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
      await fetch('/api/post', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(form)
      });

      const res = await fetch('/api/post');
      const data = await res.json();
      setPosts(data);
      setForm({ title: '', content: '' });
    } else {
      await fetch(`/api/post/${edit_id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(form)
      });

      const res = await fetch('/api/post');
      const data = await res.json();
      setPosts(data);
      set_edit_id(null);
      setForm({ title: '', content: '' });
    }
  };

  const deletePost = async (id) => {
    await fetch(`/api/post/${id}`, {
      method: 'DELETE',
    });
    const res = await fetch('/api/post');
    const data = await res.json();
    setPosts(data);
  };

  const startEdit = (p) => {
    setForm({ title: p.title, content: p.content });
    set_edit_id(p._id);
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
          <li key={p._id}>
            <strong>{p.title}</strong><br/>
            {p.content}<br/>
            <button onClick={() => deletePost(p._id)}>Delete</button>
            <button onClick={() => startEdit(p)}>Edit</button>
          </li>
        ))}
      </ul>
    </div>
  );
}

export default Board;
