export default {
  server: {
    host: true,
    port: 5173,
    proxy: {
      '/api': {
        target: 'http://backend:5001',
        changeOrigin: true,
        rewrite: path => path
      }
    }
  }
}
