import net from 'net';

// 接続先の設定
const HOST = '127.0.0.1';
const PORT = 54000;

const sql = `SELECT * FROM temp ORDER by id DESC LIMIT 10;`;
console.log("sql=", sql)
const sql_str = sql.replace(/\r?\n/g, "");

let RECV_DATA = "";
// ソケット接続を作成
const client = net.createConnection({ host: HOST, port: PORT }, () => {
  console.log(`✅ サーバーに接続しました: ${HOST}:${PORT}`);
  // サーバーへメッセージを送信
  client.write(`{"action_name": "select", "table":"todos", "sql":"${sql_str}"}`);
});
// データを受信したとき
client.on('data', (data) => {
  console.log('📩 受信データ:', data.toString());
  const rcv_data =  data.toString();
  RECV_DATA += rcv_data;
});
// 接続が終了したとき
client.on('end', () => {
  console.log('🔌 サーバーから切断されました');
});
// エラー発生時
client.on('error', (err) => {
  console.error('❌ エラー:', err.message);
});
// タイムアウト設定（任意）
client.setTimeout(1000);
client.on('timeout', () => {
  console.log('⏰ タイムアウト');
  client.destroy();

  const j1 = JSON.parse(RECV_DATA);
  console.log(j1)

});