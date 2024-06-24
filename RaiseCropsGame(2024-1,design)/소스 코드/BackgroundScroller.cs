using UnityEngine;

public class BackgroundScroller : MonoBehaviour
{
    public GameObject background1;
    public GameObject background2;
    public float scrollSpeed = 2.0f;
    private float backgroundWidth;

    void Start()
    {
        // ��� �̹����� ���� ���̸� ���
        backgroundWidth = background1.GetComponent<SpriteRenderer>().bounds.size.x;
    }

    void Update()
    {
        // ��� �̹����� �������� �̵�
        background1.transform.position += Vector3.left * scrollSpeed * Time.deltaTime;
        background2.transform.position += Vector3.left * scrollSpeed * Time.deltaTime;

        // ù ��° ����� ȭ�� ������ ������ �� ��° ����� �ڷ� �̵�
        if (background1.transform.position.x <= -backgroundWidth)
        {
            background1.transform.position = new Vector3(background2.transform.position.x + backgroundWidth, background1.transform.position.y, background1.transform.position.z);
        }

        // �� ��° ����� ȭ�� ������ ������ ù ��° ����� �ڷ� �̵�
        if (background2.transform.position.x <= -backgroundWidth)
        {
            background2.transform.position = new Vector3(background1.transform.position.x + backgroundWidth, background2.transform.position.y, background2.transform.position.z);
        }
    }
}
