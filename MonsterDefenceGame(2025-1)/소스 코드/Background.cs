using UnityEngine;

public class Background : MonoBehaviour
{
    public GameObject BackGround1;
    public GameObject BackGround2;

    public float scrollSpeed = 2f;
    private float backgroundWidth;

    void Start()
    {
        // ��������Ʈ �������� ũ��� �̹��� �ʺ� ��������
        backgroundWidth = BackGround1.GetComponent<SpriteRenderer>().bounds.size.x;

        // BackGround2�� BackGround1 ������ �ٷ� ���� ��ġ��Ű��
        BackGround2.transform.position = new Vector3(BackGround1.transform.position.x + backgroundWidth, BackGround1.transform.position.y, BackGround1.transform.position.z);
    }

    void Update()
    {
        // �� ��� ��� �������� �̵�
        BackGround1.transform.position += scrollSpeed * Time.deltaTime * Vector3.left;
        BackGround2.transform.position += scrollSpeed * Time.deltaTime * Vector3.left;

        // BackGround1�� ������ ȭ�� ���� ������ ������ BackGround2 �����ʿ� ���̱�
        if (BackGround1.transform.position.x <= -backgroundWidth)
        {
            BackGround1.transform.position = new Vector3(BackGround2.transform.position.x + backgroundWidth, BackGround1.transform.position.y, BackGround1.transform.position.z);
        }

        // BackGround2�� ������ ȭ�� ���� ������ ������ BackGround1 �����ʿ� ���̱�
        if (BackGround2.transform.position.x <= -backgroundWidth)
        {
            BackGround2.transform.position = new Vector3(BackGround1.transform.position.x + backgroundWidth, BackGround2.transform.position.y, BackGround2.transform.position.z);
        }
    }
}
