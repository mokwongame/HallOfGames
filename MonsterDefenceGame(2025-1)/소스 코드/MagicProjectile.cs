using UnityEngine;

public class MagicProjectile : MonoBehaviour
{
    public float speed = 10f;
    public int damage = 1;

    void Update()
    {
        transform.Translate(Vector2.right * speed * Time.deltaTime);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Monster"))
        {
            // 몬스터 데미지 처리
            Monster monster = collision.GetComponent<Monster>();
            if (monster != null)
            {
                monster.TakeDamage(damage);
            }

            // 마법 오브젝트 제거
            Destroy(gameObject);
        }
        else if (collision.CompareTag("Ground"))
        {
            // 땅에 닿아도 제거되도록 할 수도 있음
            Destroy(gameObject);
        }
    }
}
