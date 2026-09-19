import net from 'net';

// Server Setting
const HOST = '127.0.0.1';
const PORT = 54000;

let cont_str = `test_1`;
cont_str = cont_str.replaceAll("\n", "<br />");
const sql =`INSERT INTO temp (title) VALUES ('${cont_str}');`;  

//Connection
const client = net.createConnection({ host: HOST, port: PORT }, () => {
  console.log(`✅ サーバーに接続しました: ${HOST}:${PORT}`);
  client.write(`{"action_name": "update", "table":"temp","sql":"${sql}"}`);
});
// receive
client.on('data', (data) => {
  console.log('📩 受信データ:', data.toString());
  const rcv_data =  data.toString();
  console.log(rcv_data)
});
client.on('end', () => {
  console.log('🔌 サーバーから切断されました');
});
// error
client.on('error', (err) => {
  console.error('❌ エラー:', err.message);
});
// Timeout
client.setTimeout(1000);
client.on('timeout', () => {
  console.log('⏰ タイムアウト');
  client.destroy();
});