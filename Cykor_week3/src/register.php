<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Cykor Week 3</title>
</head>
<body>
    <h1>Register Page</h1>
    <ul>
        <li><a href = "./index.php">Main</a></li>
        <li><a href = "./login.php">Login</a></li>
        <li><a href = "./register.php">Register</a></li>
    </ul>
    <form action="./login.php" method="post">
        <label for="new_username">Username:</label>
        <input type="text" id="new_username" name="New_username">
        <br>
        <label for="new_password">Password:</label>
        <input type="password" id="new_password" name="New_password">
        <br>
        <button type="submit">Register</button>
    </form>
</body>
</html>
