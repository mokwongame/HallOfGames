using UnityEngine;

public class EnemyBullet : MonoBehaviour
{
    [SerializeField] float m_bulletSpeed;

    void Start()
    {
        Invoke("BulletDestroy", 3);
    }
    // Update is called once per frame
    void Update()
    {
        transform.Translate(Vector2.left * m_bulletSpeed * Time.deltaTime);

        RaycastHit2D m_hit = Physics2D.Raycast(transform.position, Vector2.left, m_bulletSpeed * Time.deltaTime, 1 << LayerMask.NameToLayer("Player"));
        
        if (m_hit)
        {
            PlayerController.Instance.GetDamage(3);
            BulletDestroy();
        }
    }
    void BulletDestroy()
    {
        Destroy(gameObject);
    }
}
