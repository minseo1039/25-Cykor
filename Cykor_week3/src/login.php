<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Cykor Week 3</title>
</head>
<body>
    <h1>Login Page</h1>
    <ul>
        <li><a href = "./index.php">Main</a></li>
        <li><a href = "./login.php">Login</a></li>
        <li><a href = "./register.php">Register</a></li>
    </ul>
    <form action="./login_check.php" method="post">
        <label for="username">Username:</label>
        <input type="text" id="username" name="username">
        <br>
        <label for="password">Password:</label>
        <input type="password" id="password" name="password">
        <br>
        <button type="submit">Login</button>
    </form>
</body>
</html>