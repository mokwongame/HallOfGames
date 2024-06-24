using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveRockAct : MonoBehaviour
{
    public float speed = 10.0f; // 속도 변수 추가

    // Start는 처음 프레임 업데이트 전에 호출됩니다
    void Start()
    {
        Rigidbody2D rb = GetComponent<Rigidbody2D>();
        if (rb != null)
        {
            Vector2 direction = Vector2.left; // 왼쪽 방향으로 설정
            rb.velocity = direction * speed; // 속도 설정
        }
        else
        {
            Debug.LogWarning("Rigidbody2D를 찾을 수 없습니다!");
        }

        Collider2D collider = GetComponent<Collider2D>();
        if (collider != null)
        {
            collider.isTrigger = true; // Collider를 트리거로 설정
        }
        else
        {
            Debug.LogWarning("Collider2D를 찾을 수 없습니다!");
        }
    }

    // Update는 매 프레임 호출됩니다
    void Update()
    {

    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Player"))
        {
            // Spaceship과 충돌하면 EndGameScene으로 전환
            UnityEngine.SceneManagement.SceneManager.LoadScene("EndGameScene");
        }
        else if (other.CompareTag("Missile"))
        {
            // Missile과 충돌하면 Rock을 파괴
            Destroy(gameObject);
            Destroy(other.gameObject); // 미사일도 파괴
        }
    }
}
