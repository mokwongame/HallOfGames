using System;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MovePlayer : MonoBehaviour
{
    public GameObject spear;

    GameManager gameManager;
    float xmin = -6.0f;
    float xmax = 6.0f;
    float ymin = -6.0f;
    float ymax = 6.0f;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        float xoff = Input.GetAxis("Horizontal") * gameManager.speedPlayer * Time.deltaTime;
        float yoff = Input.GetAxis("Vertical") * gameManager.speedPlayer * Time.deltaTime;
        Vector3 moveOffset = new Vector3(xoff, yoff, 0.0f);

        // 경계값 제한 (선택 사항)
        Vector3 newPos = transform.position + moveOffset;
        newPos.x = Mathf.Clamp(newPos.x, xmin, xmax);
        newPos.y = Mathf.Clamp(newPos.y, ymin, ymax); // ymin, ymax는 public 변수로 선언하거나 직접 설정

        transform.position = newPos;

        if (Input.GetKeyDown(KeyCode.Space))
        {
            makeSpear();
        }
    }

    private void makeSpear()
    {
        Vector3 pos = transform.position + Vector3.right * 3.0f;
        Instantiate(spear, pos, Quaternion.identity);
    }


    private void OnCollisionEnter2D(Collision2D collision)
    {
        string tag = collision.gameObject.tag;

        if (tag == "Stone")
        {
            Debug.Log("Player hit by Stone.");

            GameResult.endReason = EndReason.KilledByStone;  // ✅ 여기 추가
            Invoke("endGame", 1.0f); // 1초 후 씬 이동
        }
    }



    void endGame()
    {
        SceneManager.LoadScene("EndScene");
    }
}

