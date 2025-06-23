using UnityEngine;

public class Magic1 : MonoBehaviour
{
    public float speed = 10f;

    void Update()
    {
        // 오른쪽으로 날아가기
        transform.Translate(Vector2.right * speed * Time.deltaTime);
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Monster"))
        {
            Monster monster = other.GetComponent<Monster>();
            if (monster != null)
            {
                monster.TakeDamage(1);  // 데미지 1
            }

            Destroy(gameObject); // 마법 오브젝트 삭제
        }
    }
}
