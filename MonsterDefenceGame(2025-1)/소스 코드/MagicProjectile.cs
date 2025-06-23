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
            // ���� ������ ó��
            Monster monster = collision.GetComponent<Monster>();
            if (monster != null)
            {
                monster.TakeDamage(damage);
            }

            // ���� ������Ʈ ����
            Destroy(gameObject);
        }
        else if (collision.CompareTag("Ground"))
        {
            // ���� ��Ƶ� ���ŵǵ��� �� ���� ����
            Destroy(gameObject);
        }
    }
}
