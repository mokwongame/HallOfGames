using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public float moveSpeed = 5f;
    private float halfWidth;

    void Start()
    {
        halfWidth = GetComponent<SpriteRenderer>().bounds.extents.x;
    }

    void Update()
    {
        float moveX = Input.GetAxisRaw("Horizontal");
        Vector3 newPos = transform.position + Vector3.right * moveX * moveSpeed * Time.deltaTime;

        // ȭ�� ��� ���
        Vector3 screenLeft = Camera.main.ViewportToWorldPoint(new Vector3(0, 0, 0));
        Vector3 screenRight = Camera.main.ViewportToWorldPoint(new Vector3(1, 0, 0));

        // x��ǥ ����
        newPos.x = Mathf.Clamp(newPos.x, screenLeft.x + halfWidth, screenRight.x - halfWidth);

        transform.position = newPos;
    }
}
