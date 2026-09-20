const data = {
  action_name: "select",
  table: "temp",
  sql: "SELECT * FROM temp ORDER BY id DESC LIMIT 20",
};

async function sendPostRequest() {
  try {
    const response = await fetch('http://localhost:8000/api/select', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    });

    if (!response.ok) {
      throw new Error(`HTTPエラー! ステータス: ${response.status}`);
    }
    const result = await response.json();
    console.log('OK:', result);
    
  } catch (error) {
    console.error('エラーが発生しました:', error);
  }
}

// 実行
sendPostRequest();