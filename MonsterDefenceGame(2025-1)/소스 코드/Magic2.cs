using UnityEngine;

public class Magic2 : MonoBehaviour
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
                monster.TakeDamage(2);  // 데미지 2
            }

            Destroy(gameObject); // 마법 오브젝트 삭제
        }
    }
}
