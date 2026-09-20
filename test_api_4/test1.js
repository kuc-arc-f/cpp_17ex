const data = {
  action_name: "update",
  table: "temp",
  sql: "INSERT INTO temp (title) VALUES ('title20_3');",
};

async function sendPostRequest() {
  try {
    const response = await fetch('http://localhost:8000/api/update', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    });

    if (!response.ok) {
      throw new Error(`HTTPエラー! ステータス: ${response.status}`);
    }
    const result = await response.text();
    console.log('OK:', result);
    
  } catch (error) {
    console.error('エラーが発生しました:', error);
  }
}

// 実行
sendPostRequest();